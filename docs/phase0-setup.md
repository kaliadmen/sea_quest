# Phase 0 — Setup (do this once per machine)

Goal: on **every** machine you use (Windows PCs and Ubuntu VMs) you can clone `sea_quest`, open it in CLion, build, run `l01_bits_lab`, and run the tests. About 1–2 hours the first time, 20 minutes for each extra machine.

---

## A. Ubuntu VM (your main bug-hunting machine)
Sanitizers work fully here, so this is where you'll run the `BUGS/` drills.

```bash
sudo apt update
sudo apt install -y build-essential gdb cmake ninja-build git clang \
                    libclang-rt-dev valgrind nasm
```
- `build-essential` = GCC + make. `clang` = a second compiler (good for comparing warnings).
- `libclang-rt-dev` = sanitizer runtimes for Clang. Without it, Clang + ASan fails to link with `cannot find libclang_rt.asan`. If apt can't find it, try `libclang-rt-18-dev` (match your clang version: `clang --version`).
- `nasm` is for Phase 2 (you've used it before).

Check:
```bash
gcc --version && cmake --version && gdb --version | head -1
```

## B. Windows
You have two compiler options. **Install the MSVC one even if you also use MinGW**: it's the compiler most Windows software (and most of what you'll reverse) is built with, and it supports AddressSanitizer.

1. **Visual Studio 2022 Build Tools** (free): in the installer pick **"Desktop development with C++"**. That gives MSVC, the Windows SDK, and ASan.
2. **CLion bundled MinGW**: already there. Fine for running lessons. No sanitizers.
3. **Git for Windows**: https://git-scm.com. During install, accept "Checkout Windows-style, commit Unix-style" (our `.gitattributes` handles the rest).

In CLion: **Settings → Build, Execution, Deployment → Toolchains** → `+` → **Visual Studio** → it auto-detects. Drag it to the top to make it the default.

## C. GitHub access on each machine (once per machine)
Easiest is the GitHub CLI:
```bash
# Ubuntu
sudo apt install -y gh
gh auth login          # choose GitHub.com -> HTTPS -> login with browser
```
On Windows: `winget install GitHub.cli`, then `gh auth login` in a terminal.

Tell Git who you are (once per machine):
```bash
git config --global user.name  "Quinze"
git config --global user.email "your-github-email"
git config --global pull.rebase true     # cleaner history when switching machines
```

## D. Clone and open
```bash
git clone https://github.com/kaliadmen/sea_quest.git
```
CLion → **Open** → pick the `sea_quest` folder → "Trust project" → it loads the CMake project (watch the CMake tab at the bottom).

## E. First build + run
1. Top-right run dropdown → **l01_bits_lab** → green ▶. Use the Run window to type into the program.
2. Run the tests: dropdown → **All CTest** (or right-click the `libquest` folder → Run CTest). Expect `100% tests passed`.
3. Terminal alternative (any OS):
   ```bash
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ctest --test-dir build --output-on-failure
   ./build/phase1-c-core/01-bits-and-hex/l01_bits_lab        # Linux
   build\phase1-c-core\01-bits-and-hex\l01_bits_lab.exe      # Windows (Ninja/MinGW)
   ```

**Ubuntu sanity check that ASan is alive:** run `l01_bug3`. You should get a red `AddressSanitizer: stack-buffer-overflow` report. If it just prints garbage instead, sanitizers aren't on; check you're in the **Debug** profile.

## F. Debugger quick-start (CLion)
- Click in the gutter left of a line number → red dot = breakpoint.
- Run with the **bug icon** instead of ▶.
- **F8** step over · **F7** step into · **Shift+F8** step out · **F9** resume.
- Hover any variable to see its value. Right-click a variable → *View as → Hex*.
- **Memory view:** right-click a variable in the Variables pane → *Show in Memory View*. This is HxD inside your debugger. Use it in Lab 5.
- **Disassembly:** while paused, *Force Step Into* on a function with no source, or right-click → *Open Disassembly View*.

## G. The session habit (every machine, every time)
```bash
git pull                # sit down
# ... work ...
git add -A
git commit -m "L01: predictions P1-P5, fixed bug 1"
git push                # get up
```
Commit message format: `L<lesson>: <what you did>`. It turns your history into a learning log.

## Done when
- [ ] Ubuntu VM: builds, tests pass, `l01_bug3` shows an ASan report
- [ ] Windows: builds and runs `l01_bits_lab` (MSVC toolchain set up)
- [ ] `gh auth login` done on both, a test commit pushed from each
- [ ] GitHub → Actions tab shows a green check for your push
- [ ] Ticked off in `PROGRESS.md`
