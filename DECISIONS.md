# Decisions

Why the course is shaped the way it is. Newest at the top. Any change to `ROADMAP.md` gets an entry here so later sessions don't undo it.

---

### 2026-11-13 — Initial course design
- **C first, then C++ (Phase 4).** C shows what the machine is actually doing (memory, bytes, pointers), which security work needs. C++ then shows how to make those bugs impossible.
- **Security track is defensive/analytical.** Detection tools, safe parsers, hardening, mitigations, fuzzing and reversing *own* code. Matches the blue-team/SOC career path.
- **Assembly is a dedicated phase (2) plus a thread in every lesson** (`UNDER_THE_HOOD.md`), so it's familiar before it's formal.
- **DSA is woven in, not a separate project.** Each structure is built in `libquest` when a lesson needs it (hash table for log analysis, heap for ATB turn order, A* for dungeons), then compared against the STL in Phase 4.
- **GUI from Phase 5 (raylib), with a preview at the end of Phase 1.** Final game is fully graphical; security tools stay CLI (optional ImGui front end).
- **AI in Phase 6**: learn how LLMs work from the inside (tiny NN, tiny transformer in C), then use llama.cpp for NPCs and AI for art/writing.
- **One CMake project, one repo, one library.** Single CLion project; `libquest` shared by everything; CI builds Ubuntu + Windows on every push.
- **Strict warnings + sanitizers on by default** in Debug.
- **No quiz answer keys in the repo.** Claude grades; misses go into the Review Queue.
- **Repo name:** `sea_quest` (chosen by Quinze).
- Note: `.gitignore` excludes `*.bin` (for AI model files). If a lesson ever needs a `.bin` test fixture, add an exception.
