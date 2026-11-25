import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { createBrowserRouter, RouterProvider } from "react-router-dom";
import "./index.css";
import "./lib/monaco";
import { Layout } from "./components/Layout";
import { CapsProvider } from "./lib/caps";
import { CourseProvider } from "./lib/course";
import { ProgressProvider } from "./lib/progress";
import { ChapterPage } from "./pages/ChapterPage";
import { Dashboard } from "./pages/Dashboard";
import { DrillPage } from "./pages/DrillPage";
import { ExercisePage } from "./pages/ExercisePage";
import { LessonPage } from "./pages/LessonPage";
import { ProjectPage } from "./pages/ProjectPage";
import { QuizPage } from "./pages/QuizPage";
import { VisualizerList, VisualizerPage } from "./pages/VisualizerPage";

const router = createBrowserRouter([
  {
    element: <Layout />,
    children: [
      { path: "/", element: <Dashboard /> },
      { path: "/ch/:ch", element: <ChapterPage /> },
      { path: "/ch/:ch/lesson/:id", element: <LessonPage /> },
      { path: "/ch/:ch/quiz", element: <QuizPage /> },
      { path: "/ch/:ch/drill", element: <DrillPage /> },
      { path: "/ch/:ch/exercise/:id", element: <ExercisePage /> },
      { path: "/ch/:ch/project/:id", element: <ProjectPage /> },
      { path: "/visualizer", element: <VisualizerList /> },
      { path: "/visualizer/:id", element: <VisualizerPage /> },
      { path: "*", element: <div>Page not found.</div> },
    ],
  },
]);

createRoot(document.getElementById("root")!).render(
  <StrictMode>
    <CapsProvider>
      <CourseProvider>
        <ProgressProvider>
          <RouterProvider router={router} />
        </ProgressProvider>
      </CourseProvider>
    </CapsProvider>
  </StrictMode>,
);
