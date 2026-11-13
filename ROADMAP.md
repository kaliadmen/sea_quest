# sea_quest — Roadmap

Learning C and C++ by building real things: **security first, games second.** The path ends with two shippable outcomes: a **Blue Team Toolkit** for work and clients, and a **playable Final Fantasy-style JRPG vertical slice** with graphics, made in C/C++ with AI in the pipeline.

**Pace:** ~12–14 hrs/week (four short weeknight review sessions + two weekend build days). **Estimate: ~40 weeks.** Faster weeks happen, slow weeks happen (cert crunch, life). Each phase stands on its own, so a pause never wrecks the plan.

Changes to this plan get logged in `DECISIONS.md`. Where you are lives in `PROGRESS.md`.

---

## How every lesson works
| Part | Purpose (the learning science) |
|---|---|
| `README.md` | Concepts + **design decisions and why** |
| `main.c` with `KNOBS` | Interactive: change it, see what happens |
| `PREDICT.md` | Predict before running. Wrong predictions expose the gaps in your mental model, which is where learning happens |
| `BUGS/` | Planted bugs to hunt with GDB, CLion and sanitizers. Debugging *is* the job, in security and in games |
| `EXERCISES.md` | You write code; some of it goes into `libquest` |
| `QUIZ.md` | Answer from memory. Retrieval practice beats rereading every time |
| `NOTES.md` | Explain it in your own words. Claude reviews this |
| `UNDER_THE_HOOD.md` | Your C as assembly. Builds toward Phase 2 from day one |

**Boss Fight** at the end of every phase: build a small program from scratch, no notes, timed. Pass = phase done. Struggle = we know exactly what to review.

**Spaced review:** missed quiz questions return in later quizzes. Old `libquest` modules get reused in later lessons, which is review built into the work.

### Weekly rhythm
| When | Time | What |
|---|---|---|
| Mon–Thu evenings | 45–60 min | Review: quiz from memory, knob experiments, one bug, one DSA drill/week |
| Friday | off | Rest / cert study |
| Sat + Sun | 3–5 hrs each | Build: new lesson, milestone work, push for review |

---

## The four threads that run through everything
1. **libquest**: your own C library, one module per lesson, with tests. Data structures and algorithms are learned by building them *because a project needs them*.
2. **Under the Hood**: assembly views of every lesson → Phase 2 → reverse engineering your own binaries.
3. **Security mindset**: every input is hostile, every size gets checked, every parser gets fuzzed. Defensive focus: detection, analysis, hardening.
4. **Game assets accumulate**: status effects, inventory, save files, and battle math built early all end up in the final game.

---

## Phase 0 — Setup · week 1
CMake + CLion on Windows (MSVC + MinGW) and Ubuntu (GCC/Clang), sanitizers, GDB, Git on every machine, CI. → `docs/phase0-setup.md`

## Phase 1 — C Core · weeks 2–6
| # | Lesson | Concepts | libquest / DSA |
|---|---|---|---|
| 01 | **Bits & Hex Lab** | types, sizes, two's complement, overflow, bit ops, endianness, padding | `bits`, `input`, `qtest` |
| 02 | **Text Adventure Parser** | control flow, functions, strings, safe string handling, tokenizing | `str` (bounded string ops, tokenizer) |
| 03 | **Party & Inventory** | structs, arrays, pointers, pass-by-pointer, `const` | `vec` (dynamic array), sorting, binary search |
| 04 | **Memory Lab** | stack vs heap, `malloc`/`free`, leaks, use-after-free, double free (caught by ASan) | `list` (linked list), `pool` (free-list allocator) |
| 05 | **Save Files** | binary file I/O, serialization, versioning, checksums (CRC32) | `stack` (undo), `ring` (ring buffer) |
| 06 | **Multi-file Programs** | headers, translation units, linking, static libraries, how builds work | Big-O + `bench` (timing harness) |

**Milestone 1:** `hexview` (xxd/HxD-style CLI viewer with search) + **save-file editor** for your own format.
**GUI preview:** a raylib window showing your party from a save file. First pixels on screen.
**Boss Fight 1.**

## Phase 2 — Assembly & the Machine · weeks 7–9
| # | Lesson | Concepts |
|---|---|---|
| 07 | **Registers & Instructions** | x86-64 registers, mov/add/cmp/jmp, writing NASM functions called from C |
| 08 | **Stack Frames** | call/ret, prologue/epilogue, calling conventions (System V vs Windows x64), stepping in GDB and x64dbg |
| 09 | **Reading Compiler Output** | `-O0` vs `-O2`, how if/loops/switch/structs/arrays compile |
| 10 | **Mitigations** | stack canaries, ASLR, NX/DEP, PIE, CFG: seeing each in your own binaries, and why each exists |
| 11 | **Reverse Your Own Binary** | build a small license-check program, strip it, analyze it cold in Ghidra/x64dbg |

**Milestone 2:** **binary analysis write-up** of lesson 11 (portfolio piece).
**Boss Fight 2.**

## Phase 3 — C for Blue Team · weeks 10–14
| # | Lesson | Concepts | libquest / DSA |
|---|---|---|---|
| 12 | **Log Analyzer** | parsing `auth.log`, brute-force detection, time windows | `map` (hash table) |
| 13 | **File Integrity Monitor** | hashing with a vetted library (never roll your own crypto), directory walking, baselines | sorted index / BST |
| 14 | **ELF/PE Header Parser** | executable formats, sections, imports. Untrusted-input parsing done right | — |
| 15 | **Offline pcap Parser** | Ethernet/IPv4/TCP/UDP headers, network byte order (ties to Network+) | — |
| 16 | **Signatures + Fuzzing** | multi-pattern string matching; fuzz your parsers from 13–15 with libFuzzer/AFL++ and fix what breaks | `trie` |

**Milestone 3:** **Blue Team Toolkit**: one documented CLI (`sqtool log|fim|pe|pcap|scan`), CI-tested, with a README a client can follow.
**Boss Fight 3.**

## Phase 4 — Into C++ · weeks 15–19
| # | Lesson | Concepts | DSA |
|---|---|---|---|
| 17 | **Inventory, Rewritten** | classes, constructors/destructors, RAII, `std::string`, `std::vector`; side by side with your C version | your `vec` vs `std::vector` |
| 18 | **Ownership** | `unique_ptr`, `shared_ptr`, move semantics, rule of 0/3/5. The Phase 1 memory bugs, made impossible | — |
| 19 | **Generic Code** | templates, STL containers + algorithms, lambdas, iterators | benchmark `libquest` vs STL |
| 20 | **Battle Engine** | inheritance vs composition, interfaces, turn-based ATB loop | **heap / priority queue** (turn order) |
| 21 | **Data-Driven Content** | enemies/items/skills from JSON, error handling, exceptions vs error codes | — |

**Milestone 4:** console **battle simulator**: encounters designed in data files, not code. Optional: Dear ImGui front end for the Blue Team Toolkit.
**Boss Fight 4.**

## Phase 5 — Game Foundations (raylib, graphical from here on) · weeks 20–25
| # | Lesson | Concepts | DSA |
|---|---|---|---|
| 22 | **Game Loop** | window, frames, delta time, input, drawing | — |
| 23 | **Tilemaps & Camera** | overworld, collision, map files, sprite sheets | 2D grids |
| 24 | **Scenes & State** | title/field/battle/menu states, pushdown state machine | stack of states |
| 25 | **Pathfinding** | NPC/enemy movement, dungeon navigation | **graphs: BFS, Dijkstra, A\*** |
| 26 | **ATB Battle Screen** | your Phase 4 engine with graphics, animations, menus | — |
| 27 | **UI, Dialogue, Audio** | text boxes, menus, music, sound effects | queue (event/message queue) |

**Milestone 5:** **walkable town + one full battle**, playable Windows build.
**Boss Fight 5.**

## Phase 6 — AI & Advanced · weeks 26–32
| # | Lesson | Concepts |
|---|---|---|
| 28 | **Tiny Neural Net in C** | neurons, weights, matrix math, training on a toy problem, all from scratch |
| 29 | **Tiny LLM Inference** | a minimal C transformer (llama2.c-style): tokens, embeddings, attention, sampling |
| 30 | **Local LLM NPCs** | llama.cpp (C/C++) for NPC dialogue: prompts, character sheets, guardrails, offline fallback |
| 31 | **Enemy AI** | behavior trees, utility AI, boss phase patterns |
| 32 | **AI Art & Writing Pipeline** | concept art → game-ready sprites, palette/size cleanup, AI as story-bible & quest co-writer; license terms for commercial use |
| 33 | **Performance** | profiling, memory arenas, cache-friendly data, SIMD (back to assembly), threads |
| 34 | **Secure Saves** | HMAC-signed save files that resist tampering: security and gaming meet |

**Boss Fight 6.**

## Final — JRPG Vertical Slice · weeks 33–40
One town, one dungeon, party of three, ATB battles, a boss with phases, LLM-driven NPCs (toggleable), signed saves, music. Packaged Windows build + itch.io page to share with friends.

---

## What you have at the end
1. **Blue Team Toolkit** + **binary analysis write-up**: for clients, interviews, and your job
2. **Playable JRPG slice**: to share, and the foundation of the full game
3. **This repo**: months of documented, reviewed progress
