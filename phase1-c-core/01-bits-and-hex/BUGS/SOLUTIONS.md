# Bug Solutions — SPOILERS

Only open after a real attempt (15+ minutes per bug). Scroll slowly; each solution is separated by blank space.

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

---

## Bug 1 — The boss that wouldn't die
`boss_hp - damage`: both `uint16_t` values are **promoted to `int`** first, so the math gives `-50` (an int). Storing `-50` into a `uint16_t` wraps modulo 65536 → **65486**. No warning because GCC treats the narrowing of an expression built *only* from narrower types as intentional.

**Fix (a), check first:**
```c
boss_hp = (damage >= boss_hp) ? 0 : (uint16_t)(boss_hp - damage);
```
**Fix (b), library:**
```c
boss_hp = (uint16_t)lq_sat_sub_u32(boss_hp, damage);   /* #include "lq/bits.h" */
```
**Rule:** for unsigned subtraction, ask "can the right side be bigger?" *before* subtracting.

&nbsp;

&nbsp;

&nbsp;

---

## Bug 2 — The poisoned hero who fell asleep
`==` and `!=` have **higher precedence** than `&`. So:
- `status & ST_SLEEP == 0` is `status & (ST_SLEEP == 0)` → `status & 0` → always 0 → always "asleep".
- `status & ST_SLEEP != 0` is `status & (ST_SLEEP != 0)` → `status & 1` → tests **bit 0 (Poison)** instead of Sleep.

**Fix:**
```c
if ((status & ST_SLEEP) == 0) { ... }
if ((status & ST_SLEEP) != 0) { ... }   /* or just: if (status & ST_SLEEP) */
```
**Rule:** *always* parenthesize a bitwise operation inside a comparison. And never ignore `-Wparentheses`.

&nbsp;

&nbsp;

&nbsp;

---

## Bug 3 — The empty chest
`count` is `size_t` (unsigned). When `count == 0`, `count - 1` wraps to `SIZE_MAX` (18,446,744,073,709,551,615 on 64-bit). The loop runs past the end of the array. ASan reports a **stack-buffer-overflow READ of size 8** (one pointer) at the `printf` line, in variable `empty_chest`.

**Fix:** use the standard half-open loop. It's correct for 0, 1, and N:
```c
for (size_t i = 0; i < count; i++)
```
**Rule:** loop with `i < count`, never `i <= count - 1`. Be suspicious of any `- 1` on an unsigned length.

**Security note:** the same "length minus something underflows" pattern, combined with input an attacker controls, is how a lot of real out-of-bounds read/write vulnerabilities happen. Fuzzing (Lesson 16) is how defenders find these in their own code before anyone else does.
