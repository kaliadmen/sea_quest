# Lesson 01 — Predictions

**Rule:** write your prediction *before* you run anything. A wrong prediction with a good "why I was wrong" is worth more than a lucky right one. Don't erase wrong predictions; they're the record of your learning.

Knobs are at the top of `main.c`. After changing one, rebuild (CLion does it automatically when you hit Run). **Reset each knob to its default when an experiment is done** unless the next one says otherwise.

---

## P1 — Your machine vs. your VM (Lab 1)
Run Lab 1 on **Windows** and on **Ubuntu**.

| row | Prediction (Windows) | Actual (Windows) | Prediction (Ubuntu) | Actual (Ubuntu) |
|---|---|---|---|---|
| `long` bytes | | | | |
| `void*` bytes | | | | |
| byte order | | | | |

Why does `long` differ? What would break if a save file stored a `long` written on one OS and read on the other?
>

## P2 — Two's complement (Lab 2)
Type each number. Predict the **hex** and the **int8_t** row before pressing Enter.

| input | predicted hex | predicted int8_t | actual hex | actual int8_t |
|---|---|---|---|---|
| `255` | | | | |
| `-1` | | | | |
| `128` | | | | |
| `0x17F` | | | | |
| `-128` | | | | |

Pattern you noticed:
>

## P3 — The odometer (Lab 3, defaults: `uint8_t`, start 200, saturation OFF)
1. Take **300** damage. Predicted HP: ____ Actual: ____
2. Restart. Drink 10 potions (+50 each). At which potion does HP first *drop*? Predicted: ____ Actual: ____

Explain the formula that gives you the answer to #1 without running it:
>

## P4 — Turn on saturation
Set `KNOB_USE_SATURATION 1`. Repeat P3 #1 and #2.
Predicted: ____ / ____ Actual: ____ / ____

## P5 — Signed HP
Set `KNOB_HP_TYPE int8_t`, saturation OFF, start HP 100. Drink potions.
- Predicted HP after potion 1: ____ Actual: ____
- After the 10 potions, take **0** damage. Does "The hero falls..." print? Predicted: ____ Actual: ____

Why is a signed HP type arguably *worse* than unsigned here?
>

## P6 — Start HP that doesn't fit
`KNOB_HP_TYPE uint8_t`, `KNOB_START_HP 300`. Predicted starting HP: ____ Actual: ____
Now `KNOB_START_HP 1000`. Predicted: ____ Actual: ____

## P7 — Esuna (Lab 4)
Set the raw byte to `0xFF` (every status). Cast Esuna.
Predicted remaining statuses: ____ Predicted byte (hex): ____
Actual: ____

Now cast Haste on a hero with Slow. Then *toggle* bit 5 (Slow) back on. The game rule "Haste removes Slow" got bypassed. Where should a rule like that live so it can't be bypassed?
>

## P8 — Padding (Lab 5)
Before running: draw the `Hero` struct (layout 1) on paper with offsets. Predict:
- `sizeof(Hero)`: ____ padding bytes: ____ at which offsets: ____

Actual: ____

Now `KNOB_HERO_LAYOUT 2`. Predicted `sizeof`: ____ Actual: ____

## P9 — GameShark (Lab 5)
HP is 600 = `0x0258`. Its bytes are at offsets 12 and 13 (layout 1).
- Poke offset **13** with `0x27`. Predicted HP: ____ Actual: ____
- Poke offset **12** with `0x0F` (after the previous poke). Predicted HP: ____ Actual: ____
- Set gil to exactly 9,999,999 using pokes. Which offsets and bytes? Predicted: ____ Did it work? ____

## P10 — Packet decoding (Lab 6)
| input | predicted output | actual |
|---|---|---|
| IPv4 `0x45` | | |
| IPv4 `0x4F` | | |
| IPv4 `0x43` | | |
| TCP `0x12` | | |
| TCP `0x03` | | |
| TCP `0x00` | | |

Why would a parser that blindly trusts IHL = 3 be dangerous?
>
