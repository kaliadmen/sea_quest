/* =============================================================================
 * lq/input.h — reading user input SAFELY (libquest module #2, from Lesson 01)
 * =============================================================================
 * Why not just scanf("%d", &x)?
 *   - Type "abc": scanf fails, leaves "abc" in the input, and a loop that
 *     asks again reads the same garbage forever.
 *   - Type "99999999999": the result is undefined behavior (overflow).
 *   - scanf("%s", buf) has no length limit -> classic buffer overflow.
 *
 * DECISION: the pattern every serious C program uses instead:
 *   1. read a whole LINE with fgets (which knows the buffer size)
 *   2. PARSE it with strtoll, checking every way it can fail
 * Treat all input as hostile, even your own typing. That's the security mindset.
 * ========================================================================== */
#ifndef LQ_INPUT_H
#define LQ_INPUT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* Read one line into buf (size bytes), strip the trailing newline.
 * If the line is longer than the buffer, the rest is thrown away so it can't
 * leak into the next read. Returns false on end-of-input (Ctrl+D / Ctrl+Z). */
bool lq_read_line(char *buf, size_t size, FILE *in);

/* Parse text into a long long. base 10 = decimal, 16 = hex, 2 = binary,
 * 0 = auto-detect ("0x2A" -> hex, "052" -> octal(!), "42" -> decimal).
 * Fails on: empty text, junk ("12abc"), out-of-range numbers.
 * DECISION: long long, not long. `long` is only 32 bits on Windows. */
bool lq_parse_ll(const char *text, int base, long long *out);

/* Keep asking until the user types a number in [min, max].
 * Accepts decimal, 0x hex, or 0b binary. Returns false on end-of-input. */
bool lq_prompt_ll(const char *prompt, long long min, long long max, long long *out);

#endif /* LQ_INPUT_H */
