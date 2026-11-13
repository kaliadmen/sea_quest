# Lesson 01 — Exercises

Write the code yourself. If you get stuck for more than 20 minutes, write down *exactly* where you're stuck in `NOTES.md` and ask me. Saying precisely what you don't understand is a skill in itself.

## 1. New status effect (warm-up)
Status bits are full (8 used). Change `status_t` to `uint16_t` and add **Berserk** (bit 8) and **Doom** (bit 9).
- Update `STATUS_NAMES`, the menu text, and the toggle range.
- Should Esuna cure Doom? Decide, and justify it in a comment.
- What *else* breaks or needs changing when the type gets wider? (Search for every `8` in Lab 4.)

## 2. Cast Dispel
Add menu option "cast Dispel": removes all **positive** statuses (Haste, Protect, Regen) and leaves negatives. Define `ST_POSITIVE_MASK`. One line of bit logic.

## 3. Byte-order swap
In Lab 2, also print the number as **big-endian bytes** (network order). Write `uint32_t swap32(uint32_t v)` using only shifts and masks. No loops, no library calls.
- Test: `swap32(0x12345678) == 0x78563412`.
- Add it to `libquest` as `lq_bswap32` in `bits.h`/`bits.c` with a test in `tests/test_bits.c`.

## 4. See ASan catch an out-of-bounds write (Ubuntu VM)
In Lab 5, temporarily change the offset prompt's max from `sizeof hero - 1` to `sizeof hero + 16`. Rebuild in Debug, poke offset `sizeof hero + 4`.
- Copy the first 5 lines of the ASan report into `NOTES.md` and label: *bug type*, *read or write*, *size*, *line*, *which variable*.
- Put the bounds check back. Commit message: `L01: restore bounds check after ASan experiment`.

This is the defensive habit: **every number that comes from outside (user, file, network) gets validated before it's used as an index or size.**

## 5. Implement `lq_rotl32` (libquest)
A *rotate* is a shift where the bits falling off one end come back in the other. Hash functions and ciphers use it constantly.
- Add `uint32_t lq_rotl32(uint32_t v, unsigned n);` to `lq/bits.h` and implement it in `bits.c`.
- In `tests/test_bits.c`, change both `#if 0` to `#if 1`. Make all checks pass with `ctest`.
- Trap: `n == 0` and `n == 32` must not trigger undefined behavior. (Why would `v >> (32 - n)` be UB when n is 0?)
- Then look at `UNDER_THE_HOOD.md` task 3 to see what the compiler does with your rotate.

## Stretch (optional, if you're flying)
- **Lab 7: Float inspector.** Show the sign, exponent and mantissa bits of a `float` (use `memcpy` into a `uint32_t`, never pointer casting; ask me why). Why does `0.1f + 0.2f != 0.3f`?
- Make `lq_to_binary` group by 8 instead of 4 via a parameter. What tests need to change?
