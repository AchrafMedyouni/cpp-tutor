---
title: "Parallel programming: concepts"
slides: "181–191"
---

**Sequential** programming: the program is one sequence of instructions executed in order. **Parallel** programming: the program is split into **parts**, each a sequence of instructions, and the parts run **simultaneously** (several cores) or **interleaved** (one core, *time-sharing*).

Why bother (slide 182)? To solve problems **faster**, to solve **bigger** problems, to do it more **cheaply**, and to use modern multi-core processors at their full potential.

**Flynn's taxonomy** (slide 183):

| | Single Data | Multiple Data |
|---|---|---|
| **Single Instruction** | SISD: a sequential program | SIMD: vector units (SSE/AVX/NEON), GPUs |
| **Multiple Instruction** | MISD: few practical uses | **MIMD**: multi-core CPUs, compute grids |

**Architecture** (slide 184): cores on a chip, chips in a machine, machines on a network. Memory is either **shared** (one address space: easy to share data, but access must be **synchronised**) or **distributed** (message passing), or a hybrid. *For the rest of this chapter:* one machine, shared memory, one process.

**Difficulties** (slide 185):
- some algorithms are intrinsically sequential (each Fibonacci term needs the previous one);
- communication and synchronisation are hard: **busy waiting** (burning CPU while waiting), **starvation** (a thread never gets its turn), **deadlock** (threads waiting on each other forever);
- **overhead**: creating threads and synchronising them costs time.

## Programs, processes, threads

A **program** is a file: static instructions and data. A **process** is a program *being executed*: its memory, registers and open files. It exists only in memory, and the same program can run as several processes. In the slides' kitchen analogy, the **recipe** is the program, the **cook** is the processor, and the **dish being prepared** is the process. One cook can work on several dishes at once, some from the same recipe.

The operating system creates processes, **schedules** them onto processors, lets them communicate and manages their resources (memory, I/O). Each process moves between states (slide 189):

```
            birth                         termination
              │                               ▲
              ▼        election               │
           ┌──────┐ ─────────────────▶ ┌──────────┐
 delete ◀──│ Ready│                    │  Active  │
           └──────┘ ◀───────────────── └──────────┘
              ▲      end of time slice       │
              │                              │ waits for I/O or a resource
              │      I/O done / resource     ▼
              └──────────────────────── ┌──────────┐
                                        │ Blocked  │
                                        └──────────┘
```

Switching processes means saving and restoring a **context block** (registers, memory map, open files…), which is expensive: caches and virtual memory are disrupted. A **thread** (*lightweight process*) has its own program counter, registers and **stack**, but shares its process's memory and resources, so it is much cheaper to create and switch. Threads also have states and are scheduled by the OS.

::example{id="hardware-threads"}
