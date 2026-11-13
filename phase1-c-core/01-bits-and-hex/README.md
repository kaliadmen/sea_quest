# Lesson 01 — Bits & Hex Lab

**Phase 1: C Core** · Time: ~1 weekend (4–6 hrs) + weekday review · Prereq: Phase 0 setup done

> Every exploit, save file, packet, and GameShark code is just someone deciding what a pattern of bits *means*.

## What you'll be able to do after this lesson
- Explain why `long` is 8 bytes on your Ubuntu VM and 4 bytes on Windows, and why that matters
- Convert between decimal, hex, and binary in your head for small numbers
- Predict exactly what happens when an integer overflows (and prevent it)
- Pack and unpack yes/no flags with `|`, `&`, `~`, `^`, `<<`, `>>`
- Read a struct's raw bytes in a hex dump, including padding and little-endian ordering
- Decode real packet fields (IPv4, TCP flags) with shift-and-mask
- Read your first AddressSanitizer report

## Order of work
| Step | File | Time |
|---|---|---|
| 1 | This README (concepts below) | 15 min |
| 2 | Run `l01_bits_lab`, play with every lab | 30 min |
| 3 | `PREDICT.md`: predict → change knob → run → compare | 60–90 min |
| 4 | `BUGS/`: three planted bugs | 45–60 min |
| 5 | `EXERCISES.md`: write code | 60–90 min |
| 6 | `UNDER_THE_HOOD.md`: see your code as assembly | 20 min |
| 7 | `NOTES.md` in your own words, then commit + push | 20 min |
| Weekdays | `QUIZ.md` from memory (no peeking), one question set per night | 15 min |

## Core concepts

### 1. A byte is 8 bits; hex is shorthand for bits
One hex digit = exactly 4 bits (a "nibble"). That's why hex is everywhere in security: `0x2A` = `0010 1010`. Memorize 0–F in binary this week; it pays off for years.

| hex | bin | hex | bin |
|---|---|---|---|
| 0 | 0000 | 8 | 1000 |
| 1 | 0001 | 9 | 1001 |
| 2 | 0010 | A | 1010 |
| 3 | 0011 | B | 1011 |
| 4 | 0100 | C | 1100 |
| 5 | 0101 | D | 1101 |
| 6 | 0110 | E | 1110 |
| 7 | 0111 | F | 1111 |

### 2. Types decide meaning
The bits `1111 1111` are 255 as `uint8_t` and -1 as `int8_t`. Nothing changed but the type. Negative numbers use **two's complement**: flip all bits and add 1. That's why `-1` is all ones.

### 3. Overflow: unsigned wraps, signed is undefined
- **Unsigned** math wraps modulo 2ⁿ, *by definition*. `uint8_t` 0 − 1 = 255. Legal, and usually a bug.
- **Signed** overflow (e.g. `INT_MAX + 1`) is **undefined behavior**. The optimizer is allowed to assume it never happens, which can delete your safety checks. UBSan catches it.
- **Integer promotion:** anything smaller than `int` is converted to `int` before math. So `uint16_t a - b` is computed as an `int` (it can be negative!) and only wraps when you store it back. Bug 1 is about this.

**Security link:** if a program computes `count * size` for a buffer and it wraps to a small number, it allocates a tiny buffer and then writes a big amount into it. That's a heap overflow. Defensive rule: *check before the math* (see `lq_sat_add_u32` in `libquest/src/bits.c`).

### 4. Bit operations cheat sheet
| Goal | Code | Mnemonic |
|---|---|---|
| turn bit ON | `x \|= MASK` | OR adds |
| turn bit OFF | `x &= ~MASK` | AND with "everything except" |
| flip bit | `x ^= MASK` | XOR toggles |
| test bit | `(x & MASK) != 0` | **always parenthesize** (Bug 2) |
| get field | `(x >> shift) & ((1u << width) - 1)` | slide down, then mask |

### 5. Endianness
x86 is **little-endian**: a number's *lowest* byte is stored at the *lowest* address. `600` = `0x0258` sits in memory as `58 02`. Network protocols use **big-endian** ("network byte order"). Lab 5 and Lab 2 show this live. In x64dbg and HxD, you're looking at little-endian data.

### 6. Struct padding
The compiler inserts invisible bytes so each field starts at an address that's a multiple of its size (its *alignment*). Lab 5 fills padding with `0xCC` so you can see it. Reordering fields largest-first shrinks the struct (`KNOB_HERO_LAYOUT 2`). This matters when you parse binary file formats: you **can't** just `fread` into a struct and trust the layout. Lesson 05 deals with that properly.

## Design decisions in this lesson (and why)
- **`<stdint.h>` types everywhere** instead of `int`/`long`: exact sizes on every platform.
- **`fgets` + `strtoll` instead of `scanf`**: scanf can't safely handle bad input. See `libquest/include/lq/input.h`.
- **Buffer functions take the buffer size** (`lq_to_binary(value, width, out, out_size)`): a function that writes into a buffer without knowing its size is a vulnerability waiting to happen.
- **Bounds check on the memory poke** (Lab 5): user-controlled offsets must be validated. Always.
- **Sanitizers on in Debug**: you'll see bugs the moment they happen instead of three functions later.

## Files
```
01-bits-and-hex/
├── README.md          you are here
├── main.c             the interactive lab, KNOBS at the top
├── PREDICT.md         predict -> run -> compare
├── EXERCISES.md       code you write
├── QUIZ.md            answer from memory, push, I grade it
├── NOTES.md           your explanation in your own words
├── UNDER_THE_HOOD.md  your C as assembly
└── BUGS/              three planted bugs + SOLUTIONS.md (spoilers)
```
Library code this lesson added: `libquest/include/lq/bits.h`, `lq/input.h`, `lq/qtest.h`.

## Done when
- [ ] All PREDICT experiments filled in
- [ ] All 3 bugs fixed, with explanations in NOTES.md
- [ ] EXERCISES 1–5 done, `ctest` passes (including your `lq_rotl32` test)
- [ ] QUIZ answered from memory
- [ ] `PROGRESS.md` + `LOG.md` updated, pushed
