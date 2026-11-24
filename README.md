# CppLab

A local web app that teaches CentraleSupélec's **C++ course (3MD1080, D. Marcadet)** in English. It follows the slides' order and examples, and every example can be edited and run with your real compiler. It also has quizzes, auto-graded exercises, step-by-step mini-projects (many themed around quantitative finance), a memory visualizer and progress tracking.

## Quick start

```bash
npm install
npm run dev          # server on 127.0.0.1:5174 + web app on http://127.0.0.1:5173
```

Open **http://127.0.0.1:5173**.

Other commands:

| Command | What it does |
|---|---|
| `npm run verify-content` | Compiles and tests **all** course content (see below). `-- 03-memory` restricts it to one chapter; `--only=examples,exercises,projects,quizzes,viz,lessons` restricts the kind; `-j 8` sets parallelism. |
| `npm run build && npm start` | Production build, served by the backend alone on http://127.0.0.1:5174 |
| `npm run typecheck` | TypeScript checks for server and web |

## Requirements

- **Node.js ≥ 20** (developed with Node 25).
- A **C++20 compiler**: GCC ≥ 11 or Clang ≥ 15. The server uses `$CXX` if it is set, otherwise `g++`, otherwise `clang++`. On macOS, `g++` *is* Apple clang (install with `xcode-select --install`). The detected compiler and its capabilities are shown at the bottom of the sidebar. If no compiler is found, a red banner explains what to install.
- Optional: **oneTBB** (`brew install tbb` / `sudo apt install libtbb-dev`) for the oneTBB part of chapter 8. Lessons that need it show a notice when it's missing, and `verify-content` skips them.

## How code is run

Every *Run* / *Check* request:

1. Writes your files to a fresh temp directory (file names are validated: no paths, only `.cpp/.cc/.h/.hpp`).
2. Compiles **each `.cpp` separately** to an object file, then links them. Separate compilation and link errors behave exactly as in the course. Flags: `-std=c++20 -Wall -Wextra -pedantic -g -pthread`, plus
   - `-fsanitize=address,undefined` (*ASan + UBSan*) or `-fsanitize=thread` (*TSan*). These can't be combined, hence the 3-way selector.
   - `-fexperimental-library`, added automatically when the code includes `<execution>` and the standard library needs it (libc++).
   - TBB include/link flags when the code includes `<tbb/...>` and TBB was detected.
3. Runs the program with your stdin, a **10 s** time limit (compilation: 20 s), and a **64 KB** output cap. The whole process group is killed on timeout.
4. Returns compiler diagnostics in structured form. They appear as markers in the editor and as a clickable list.

The server binds to **127.0.0.1 only** and rejects requests whose `Host` header isn't localhost (DNS-rebinding protection). It still runs arbitrary code you type, with your user's permissions. That is the point of a local learning tool, but don't expose it.

## Content is data

All course material lives in `content/`. You can edit it or add to it without touching React code: the server reads it from disk on every request.

```
content/
  course.json                      ordered list of chapter folders
  <NN-chapter>/
    chapter.json                   number, title, slide range, summary
    lessons/NN-name.md             Markdown + frontmatter {title, slides}
    examples/<id>/                 main.cpp (+ more files) and meta.json
    quiz.json                      questions with explanations (+ optional machine checks)
    exercises/NN-name/             exercise.json, statement.md, starter/, solution/, tests/ or io-tests.json
    projects/NN-name/              project.json, starter/, steps/<n>/{statement.md, tests/, solution/}
  visualizer/<scenario>.json       hand-authored memory states, one per step
```

Lesson Markdown supports these directives:

- `::example{id="two-files"}` embeds a runnable example. Use `id="other-chapter/name"` to reference another chapter's example.
- `::visualize{id="slicing"}` links to a memory-visualizer scenario.
- `:::note`, `:::tip`, `:::warning`, `:::course` (remarks and errata about the slides), `:::modern` and `:::finance` render as callouts.

Key fields of an example's `meta.json`: `title`, `files` (tab order), `stdin`, `expectedStdout`, `sanitizer` (pre-selected in the UI), `trackAllocs`, `defines`, `tryThis[]`, `courseVersion` (the code reproduces a slide verbatim), `requires` (`par`/`tbb`/`tsan`) and `expect`:

| `expect` | Meaning, as checked by `verify-content` |
|---|---|
| `clean` (default) | zero warnings; runs clean under ASan+UBSan (or `verifyWith`); no leak; stdout matches `expectedStdout` |
| `compile-error` | must fail to compile or link |
| `warning` | must compile with at least one warning |
| `ub` | ASan/UBSan must report an error |
| `leak` | the allocation tracker must report a leak |
| `race` | ThreadSanitizer must report a data race |

### Exercises and grading

- `mode: "doctest"` (default): your files are compiled with the hidden `tests/*.cpp`, which use [doctest](https://github.com/doctest/doctest) (vendored in `server/cpp-support/doctest.h`). They are linked with a precompiled doctest `main` (cached in `.cache/`), and results come from doctest's XML reporter.
- `mode: "io"`: `io-tests.json` lists `{name, stdin, expectedStdout, match: exact|trim|regex}`.
- Tests can check behaviour rather than output. `cpplab/tracked.h` provides `cpplab::Tracked`, which counts constructions, copies, moves and destructions. `cpplab/alloc_tracker.h` provides `cpplab::live_allocations()`, which counts heap blocks still allocated.
- Only the files listed in `editable` are taken from the learner. Other starter files are read-only.
- The reference solution unlocks after one graded attempt.

## verify-content

`npm run verify-content` fails (non-zero exit) when any of these is false:

- every `clean` example compiles **without warnings** under `-Wall -Wextra -pedantic` and runs clean under sanitizers, and every bug-demo example actually exhibits its bug;
- every exercise's reference solution passes all its tests under its sanitizer with zero warnings, and the **starter fails** at least one test (no vacuous tests);
- every project step's solution passes that step's tests, plus earlier steps' tests when `cumulative`;
- every quiz question with a `verify` block has the claimed behaviour (compiles or not, prints X, triggers a sanitizer);
- every `::example`/`::visualize` reference resolves, every example is used by a lesson, and every visualizer step is consistent.

## Design decisions

- **Backend: Node + Express (TypeScript)** rather than FastAPI. The frontend is TypeScript, so one toolchain and one `npm install` cover everything. The content schemas (zod) and API types in `shared/` are shared by the server, the web app and the verify script. `child_process.spawn` with process groups gives reliable timeouts.
- **Compiler: whatever `g++` is.** On macOS that's Apple clang, which supports ASan, UBSan and TSan. Homebrew GCC doesn't support the sanitizers on Apple Silicon. Override with `CXX=...`.
- **Leak detection uses an allocation tracker, not LeakSanitizer.** LSan isn't supported on macOS. `cpp-support/cpplab/alloc_tracker.cpp` replaces global `operator new/delete` with counting versions, and is linked when *Track allocations* is on and always during grading. It forwards to `malloc`/`free` without headers, so ASan still reports double frees etc. at the right address.
- **`std::execution::par`** works on libc++ only with `-fexperimental-library`, which CppLab detects and adds. With libstdc++ (GCC), parallel policies need TBB, which is also detected.
- **Progress** is stored in `data/progress.json` through the backend, with a localStorage backup, so it survives browser resets. *Reset all progress* is on the dashboard.
- **Monaco is bundled locally** (no CDN), with only the C++ language.
- **Memory visualizer:** hand-authored JSON states rendered with custom SVG rather than React Flow. It gives full control over layout and readability and adds no dependency.
- **Course PDFs** go in `course/` and are git-ignored (school material).
- **No memory limit** on programs: macOS ignores `RLIMIT_AS`. Time and output caps still apply.
