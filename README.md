# sea_quest

**Learning C and C++ by building real things: security tools first, a Final Fantasy-style JRPG second.**

Every lesson is an interactive, heavily commented program with knobs to turn, bugs to hunt, and its own assembly walkthrough. Reusable code accumulates in **libquest**, a C library with tests. The path ends with a **Blue Team Toolkit** and a **playable JRPG vertical slice**.

| | |
|---|---|
| 🗺️ **Plan** | [`ROADMAP.md`](ROADMAP.md) |
| 📍 **Where I am** | [`PROGRESS.md`](PROGRESS.md) |
| 📝 **Session log** | [`LOG.md`](LOG.md) |
| 🧭 **Why it's built this way** | [`DECISIONS.md`](DECISIONS.md) |
| 🛠️ **Setup (start here)** | [`docs/phase0-setup.md`](docs/phase0-setup.md) |

## Layout
```
sea_quest/
├── ROADMAP.md  PROGRESS.md  LOG.md  DECISIONS.md  CLAUDE.md
├── CMakeLists.txt          one project, opens in CLion
├── cmake/                  shared warnings + sanitizer settings
├── docs/                   setup guides
├── libquest/               my C library: include/lq/*.h, src/, tests/
└── phase1-c-core/
    └── 01-bits-and-hex/    Lesson 01
```

## Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```
Builds on Ubuntu (GCC/Clang) and Windows (MSVC/MinGW). CI checks both on every push.
