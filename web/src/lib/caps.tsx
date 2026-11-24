import { createContext, useContext, type ReactNode } from "react";
import type { Capabilities } from "./api";
import { api } from "./api";
import { useAsync } from "./useAsync";

const CapsContext = createContext<{ caps?: Capabilities; error?: string }>({});

export function CapsProvider({ children }: { children: ReactNode }) {
  const { data, error } = useAsync(() => api.health(), []);
  return <CapsContext.Provider value={{ caps: data?.capabilities, error }}>{children}</CapsContext.Provider>;
}

export const useCaps = () => useContext(CapsContext);

export function missingCaps(caps: Capabilities | undefined, required: string[] | undefined): string[] {
  if (!caps || !required) return [];
  return required.filter((r) => {
    if (r === "par") return !caps.par.available;
    if (r === "tbb") return !caps.tbb.available;
    if (r === "tsan") return !caps.tsan;
    if (r === "asan_ubsan") return !caps.asanUbsan;
    return false;
  });
}

export const CAP_HELP: Record<string, string> = {
  tbb: "oneTBB is not installed. Install it with `brew install tbb` (macOS) or `sudo apt install libtbb-dev` (Linux), then restart `npm run dev`.",
  par: "Your compiler's standard library does not support std::execution policies.",
  tsan: "ThreadSanitizer is not available with this compiler.",
  asan_ubsan: "AddressSanitizer/UBSan are not available with this compiler.",
};
