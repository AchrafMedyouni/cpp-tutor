import { createContext, useContext, type ReactNode } from "react";
import { api, type Overview } from "./api";
import { useAsync } from "./useAsync";

const CourseContext = createContext<{ course?: Overview; error?: string }>({});
export function CourseProvider({ children }: { children: ReactNode }) {
  const { data, error } = useAsync(() => api.course(), []);
  return <CourseContext.Provider value={{ course: data, error }}>{children}</CourseContext.Provider>;
}
export const useCourse = () => useContext(CourseContext);
