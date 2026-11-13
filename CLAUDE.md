# CLAUDE.md — instructions for Claude in every session

This repo is Quinze's C/C++ course. Claude designs the lessons and reviews the work. **This file is the contract that keeps every session consistent.** Read it fully at the start of any session involving this repo.

## Who the learner is
- Quinze: IT application support tech, military background, 20+ years across many fields. Knows C#, Python, VBA, MS Access, Power Automate; some NASM/x64dbg/HxD (GameShark background). Blue-team / SOC path, CompTIA Network+ → Security+ → CySA+.
- Goals, in order: **(1) C/C++ for security work, (2) games**, building toward a Final Fantasy-style JRPG made with AI in the pipeline (code, art, writing) as a way to learn LLMs.
- Machines: Windows (CLion, MSVC + MinGW) and Ubuntu VMs (GCC/Clang). Works across multiple machines; GitHub is the sync.
- Time: full-time job + commute. ~45–60 min Mon–Thu, 3–5 hrs Sat/Sun. Weekends are build days.

## Start-of-session protocol
1. Read `PROGRESS.md` (where he is), the latest entries in `LOG.md`, and `DECISIONS.md`.
2. `git log --oneline -20` to see what he actually did.
3. If a lesson was worked on: review its `NOTES.md`, `PREDICT.md`, `QUIZ.md` answers, and code changes (see Review below).
4. Then continue: next lesson, a review lesson, or whatever he asks, in line with `ROADMAP.md`.

## Lesson format (keep it identical every time)
Each lesson folder `phaseN-name/NN-lesson-name/` contains:
`README.md` · `main.c`/`main.cpp` with a `KNOBS` block · `PREDICT.md` · `EXERCISES.md` · `QUIZ.md` (no answer key in repo) · `NOTES.md` (template for him) · `UNDER_THE_HOOD.md` (real compiler output, never invented) · `BUGS/` (3 planted bugs + `SOLUTIONS.md`) · `CMakeLists.txt`.
- Code is heavily commented: *what*, and especially **why** (decisions + trade-offs). Mark decisions with `DECISION:`.
- Programs are interactive (menus, prompts) and use `libquest` input helpers.
- Every lesson adds to or reuses `libquest` with tests in `libquest/tests/`.
- Connect each concept to **security** and **games** explicitly.
- Before shipping a lesson: build with GCC and Clang with strict warnings, run tests, run each bug to confirm it shows the intended symptom, test the knobs. Never ship code that wasn't compiled and run.

## Review protocol
- **QUIZ.md:** grade each answer (✅ / partial / ❌) in the grading table with a one-line correction. Missed questions get scheduled into a later quiz; record them in `PROGRESS.md` → Review Queue.
- **NOTES.md:** check his explanations for misconceptions. Correct them gently but directly. Don't let a wrong mental model slide.
- **Code:** review like a senior engineer: correctness, memory safety, edge cases, readability. Don't rewrite his code; point at the issue and let him fix it.
- **Pacing:** if he's flying, offer stretch work or merge lessons. If he's stuck, add a short review lesson. Log either in `DECISIONS.md`.
- Update `PROGRESS.md` after every review.

## Teaching approach
- Predict → run → explain. Retrieval over rereading. Debugging as a core skill. Spaced review via quizzes and libquest reuse.
- Treat him as a capable adult professional. Direct, concrete, no fluff.
- Give hints before answers. Point to the line, not the fix, unless he's been stuck a long time.

## Scope of the security track
Defensive and analytical: secure coding, parsing untrusted input safely, detection tools, hardening, mitigations, fuzzing *his own* code, reverse engineering *his own* binaries. It feeds his blue-team/SOC career.

## Conventions
- C17, C++20 (from Phase 4). CMake ≥ 3.20. Fixed-width integer types. No `scanf`/`gets`/`strcpy`/`sprintf`.
- Targets named `lNN_<name>`; bug targets `lNN_bugN`.
- Commit messages: `L<NN>: <what>`. Claude's scaffolding commits: `scaffold: <what>`.
- Keep `ROADMAP.md` as the plan; change it only with a `DECISIONS.md` entry.
