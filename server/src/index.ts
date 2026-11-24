import { existsSync } from "node:fs";
import { mkdir, readFile, rename, writeFile } from "node:fs/promises";
import path from "node:path";
import express, { type NextFunction, type Request, type Response } from "express";
import { getCapabilities } from "./compiler/detect.js";
import { REPO_ROOT } from "./compiler/pipeline.js";
import {
  HttpError,
  loadExample,
  loadExercise,
  loadExerciseInternals,
  loadLesson,
  loadOverview,
  loadProject,
  loadProjectStepInternals,
  loadQuiz,
  listViz,
  loadViz,
  safeId,
} from "./content/loader.js";
import { gradeExercise, gradeProjectStep, runProgram } from "./services.js";

const HOST = "127.0.0.1"; // never expose the compile server on the network
const PORT = Number(process.env.CPPLAB_PORT ?? 5174);
const PROGRESS_FILE = path.join(REPO_ROOT, "data", "progress.json");

const app = express();
app.use(express.json({ limit: "1mb" }));

// Only accept requests addressed to localhost (defends against DNS rebinding).
app.use((req, res, next) => {
  const host = (req.headers.host ?? "").split(":")[0];
  if (!["127.0.0.1", "localhost"].includes(host)) return res.status(403).json({ error: "forbidden host" });
  next();
});

type Handler = (req: Request, res: Response) => Promise<unknown>;
const h = (fn: Handler) => (req: Request, res: Response, next: NextFunction) =>
  fn(req, res)
    .then((data) => {
      if (!res.headersSent) res.json(data);
    })
    .catch(next);

// Simple concurrency limit for compile jobs.
let active = 0;
const queue: (() => void)[] = [];
const MAX_JOBS = 4;
async function limited<T>(fn: () => Promise<T>): Promise<T> {
  if (active >= MAX_JOBS) await new Promise<void>((r) => queue.push(r));
  active++;
  try {
    return await fn();
  } finally {
    active--;
    queue.shift()?.();
  }
}

const p = (req: Request, k: string) => safeId(String(req.params[k]));

app.get("/api/health", h(async () => ({ ok: true, capabilities: await getCapabilities() })));
app.get("/api/course", h(() => loadOverview()));
app.get("/api/chapters/:ch/lessons/:id", h((req) => loadLesson(p(req, "ch"), p(req, "id"))));
app.get("/api/chapters/:ch/examples/:id", h((req) => loadExample(p(req, "ch"), p(req, "id"))));
app.get("/api/chapters/:ch/quiz", h((req) => loadQuiz(p(req, "ch"))));
app.get(
  "/api/chapters/:ch/exercises/:id",
  h(async (req) => {
    const { dir: _dir, ...ex } = await loadExercise(p(req, "ch"), p(req, "id"));
    return ex;
  }),
);
app.get(
  "/api/chapters/:ch/exercises/:id/solution",
  h(async (req) => {
    const ex = await loadExerciseInternals(p(req, "ch"), p(req, "id"));
    return { files: ex.solution };
  }),
);
app.get(
  "/api/chapters/:ch/projects/:id",
  h(async (req) => {
    const { dir: _dir, ...pr } = await loadProject(p(req, "ch"), p(req, "id"));
    return pr;
  }),
);
app.get(
  "/api/chapters/:ch/projects/:id/steps/:step/solution",
  h(async (req) => {
    const s = await loadProjectStepInternals(p(req, "ch"), p(req, "id"), Number(req.params.step));
    return { files: s.solution };
  }),
);
app.get("/api/visualizer", h(() => listViz()));
app.get("/api/visualizer/:id", h((req) => loadViz(p(req, "id"))));

app.post("/api/run", h((req) => limited(() => runProgram(req.body))));
app.post(
  "/api/grade",
  h((req) => {
    const { kind, id, step, files } = req.body ?? {};
    const [ch, ex] = String(id ?? "").split("/");
    if (!Array.isArray(files)) throw new HttpError(400, "files missing");
    if (kind === "exercise") return limited(() => gradeExercise(safeId(ch), safeId(ex), files));
    if (kind === "project") return limited(() => gradeProjectStep(safeId(ch), safeId(ex), Number(step), files));
    throw new HttpError(400, "kind must be exercise or project");
  }),
);

app.get(
  "/api/progress",
  h(async () => (existsSync(PROGRESS_FILE) ? JSON.parse(await readFile(PROGRESS_FILE, "utf8")) : {})),
);
app.put(
  "/api/progress",
  h(async (req) => {
    if (typeof req.body !== "object" || req.body === null) throw new HttpError(400, "bad progress");
    await mkdir(path.dirname(PROGRESS_FILE), { recursive: true });
    const tmp = `${PROGRESS_FILE}.tmp`;
    await writeFile(tmp, JSON.stringify(req.body, null, 2));
    await rename(tmp, PROGRESS_FILE);
    return { ok: true };
  }),
);

// Serve the built frontend when present (npm start after npm run build).
const DIST = path.join(REPO_ROOT, "web", "dist");
if (existsSync(DIST)) {
  app.use(express.static(DIST));
  app.get(/^(?!\/api\/).*/, (_req, res) => res.sendFile(path.join(DIST, "index.html")));
}

app.use((err: unknown, _req: Request, res: Response, _next: NextFunction) => {
  const status = err instanceof HttpError ? err.status : 500;
  const message = err instanceof Error ? err.message : String(err);
  if (status === 500) console.error(err);
  res.status(status).json({ error: message });
});

app.listen(PORT, HOST, async () => {
  console.log(`CppLab server on http://${HOST}:${PORT}`);
  const caps = await getCapabilities();
  if (!caps.compiler) console.error(`⚠️  ${caps.error}`);
  else {
    console.log(`  compiler: ${caps.compiler.version} (${caps.compiler.path})`);
    console.log(
      `  asan+ubsan: ${caps.asanUbsan}  tsan: ${caps.tsan}  execution::par: ${caps.par.available} ${caps.par.extraFlags.join(" ")}  tbb: ${caps.tbb.available}`,
    );
  }
});
