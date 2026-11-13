# Lesson 01 — Quiz

**Answer from memory. No notes, no running code, no searching.** Write your answer under each question, even if it's "I don't know" or a guess. Guessing and then being corrected still builds memory. Push it; I'll grade it and the questions you miss come back in later quizzes.

Suggested: questions 1–5 one weeknight, 6–10 the next. Re-answer any misses a few days later.

---

1. How many bits is one hex digit? Write `0xB7` in binary.
>

2. What is `sizeof(long)` on 64-bit Windows vs. 64-bit Linux? Why should a file format never use `long`?
>

3. A `uint8_t` holds 10. You subtract 20. What value does it hold, and why is that *defined* behavior?
>

4. An `int` holds `INT_MAX`. You add 1. What does the C standard say happens? Why is that scarier than question 3?
>

5. Write the one-line expression to (a) turn ON bit 4 of `flags`, (b) turn it OFF, (c) test it.
>

6. What does `flags & 0x02 == 0` actually compute? How do you write what was intended?
>

7. The number `0x0258` is stored at address 100 on an x86 machine. What byte is at address 100? At 101?
>

8. Why might `sizeof(struct)` be larger than the sum of its fields? How can you reduce it?
>

9. Why is `if (a + b > limit)` an unsafe overflow check for unsigned `a` and `b`? Write a safe version.
>

10. A `size_t count` is 0. What is `count - 1`? Name one real-world consequence when that value is used as a loop bound or a length.
>

---
*Grading (filled in by Claude on review):*
| Q | Result | Comes back in |
|---|---|---|
| | | |
