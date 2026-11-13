# Lesson 01 — Under the Hood

Every lesson ends with a look at what your C turns into. By Phase 2 (Assembly), none of it will look alien.

**Tools:**
- **Compiler Explorer** (godbolt.org): paste C on the left, see assembly on the right. Pick `x86-64 gcc` and add flags `-O0` or `-O2 -masm=intel`.
- **Local, Ubuntu:** `gcc -O2 -S -masm=intel file.c -o -` prints assembly to the terminal.
- **Local, Windows:** open your built `.exe` in **x64dbg** (you already have it). It's the same instructions, different calling convention (Phase 2 explains).

Below is real output from GCC 13 on x86-64 Linux, Intel syntax (`instruction destination, source`).

## 1. Testing a status flag
```c
bool has_status(uint8_t flags, uint8_t mask) { return (flags & mask) != 0; }
```
**-O0 (no optimization, what Debug builds look like):**
```asm
has_status:
    push  rbp                      ; set up a stack frame
    mov   rbp, rsp
    mov   edx, edi                 ; arg 1 (flags) arrives in edi
    mov   eax, esi                 ; arg 2 (mask)  arrives in esi
    mov   BYTE PTR -4[rbp], dl     ; store both on the stack...
    mov   BYTE PTR -8[rbp], al
    movzx eax, BYTE PTR -4[rbp]    ; ...and load them right back
    and   al, BYTE PTR -8[rbp]     ; flags & mask
    test  al, al                   ; is the result zero?
    setne al                       ; al = 1 if NOT equal to zero
    pop   rbp
    ret                            ; return value is in al/eax
```
**-O2 (optimized, what Release builds look like):**
```asm
has_status:
    test  sil, dil      ; AND the two args, set flags, throw away the result
    setne al            ; al = 1 if the AND was nonzero
    ret
```
**Notice:** 12 instructions become 3. `test` *is* an AND that only sets CPU flags. Your `(flags & mask) != 0` maps directly onto it. This is why Debug builds are easy to read in a debugger (every variable lives on the stack) and Release builds are hard. When you reverse-engineer real software, it's almost always the -O2 kind.

## 2. Saturating subtract, with no branch
```c
uint32_t sat_sub(uint32_t a, uint32_t b) { return (b > a) ? 0u : a - b; }
```
```asm
sat_sub:
    mov   eax, edi      ; eax = a
    xor   edx, edx      ; edx = 0   (xor with itself = classic "set to zero")
    sub   eax, esi      ; eax = a - b   (may wrap, we don't care yet)
    cmp   edi, esi      ; compare a with b
    cmovb eax, edx      ; if a < b (unsigned "below"), eax = 0
    ret
```
**Notice:** no `if`, no jump. `cmov` = *conditional move*. The CPU computes the wrapped result anyway, then replaces it with 0 if needed. `b` in `cmovb` means **b**elow, the *unsigned* comparison. Signed comparisons use `l` (less). The CPU itself doesn't know if a number is signed; the *instruction* decides. Same lesson as Lab 2, at the hardware level.

## 3. Raw 16-bit subtract (Bug 1's line)
```c
uint16_t raw_sub(uint16_t a, uint16_t b) { return (uint16_t)(a - b); }
```
```asm
raw_sub:
    mov   eax, edi
    sub   eax, esi      ; 32-bit subtract (integer promotion, in hardware!)
    ret                 ; caller only looks at the low 16 bits (ax)
```
**Notice:** there's no 16-bit "wrap" instruction. The math happens in a 32-bit register, and the truncation is just *the caller ignoring the top half*. Integer promotion isn't an abstract rule; it's what the hardware does.

## 4. Setting a bit
```c
uint32_t set_bit(uint32_t v, unsigned n) { return v | (1u << n); }
```
```asm
set_bit:
    mov   eax, edi
    bts   eax, esi      ; Bit Test and Set: one instruction does it all
    ret
```
**Notice:** the compiler recognized your pattern and used a dedicated x86 instruction. Optimizers are very good at spotting idioms, which is a reason to write clear idiomatic C instead of "clever" C.

## Your turn
1. Paste `lq_popcount` from `libquest/src/bits.c` into Compiler Explorer with `-O2`. Then add `-march=native` (or `-mpopcnt`). What happens to the whole loop? Write it in NOTES.md.
2. Paste Bug 2's broken `if` and the fixed one. Compare the assembly. Can you see where the compiler evaluated `ST_SLEEP == 0` at compile time?
3. After Exercise 5, paste your `lq_rotl32` with `-O2`. If you wrote it the standard way, the compiler turns it into a single `rol` instruction. Did yours?
