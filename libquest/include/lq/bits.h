/* =============================================================================
 * lq/bits.h — bit manipulation helpers (libquest module #1, from Lesson 01)
 * =============================================================================
 * A header (.h) is the PROMISE: names, parameter types, return types.
 * The .c file is the DELIVERY: the actual code. Other files only need the
 * promise to call these functions; the linker connects them to the delivery.
 *
 * DECISION: fixed-width types (uint8_t, uint32_t...) from <stdint.h> everywhere.
 * Plain `int` / `long` change size between platforms. `long` is 4 bytes on
 * Windows and 8 bytes on 64-bit Linux. Lesson 01 shows you this live. When
 * you're reading file formats, packets, or save files, the exact size matters,
 * so we say exactly what we mean.
 * ========================================================================== */
#ifndef LQ_BITS_H
#define LQ_BITS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* ---- Single bits -----------------------------------------------------------
 * `n` is the bit position: 0 = rightmost (least significant) bit.
 * These return a NEW value; they don't modify anything in place. Pure
 * functions are easy to test and impossible to misuse with a bad pointer. */
bool     lq_bit_test(uint32_t value, unsigned n);
uint32_t lq_bit_set(uint32_t value, unsigned n);
uint32_t lq_bit_clear(uint32_t value, unsigned n);
uint32_t lq_bit_toggle(uint32_t value, unsigned n);

/* How many bits are 1? (a.k.a. "population count"). */
unsigned lq_popcount(uint32_t value);

/* ---- Fields -----------------------------------------------------------------
 * Pull `width` bits starting at bit `shift` out of `value`.
 * Example: an IPv4 header's first byte holds two 4-bit fields:
 *   version = lq_bits_extract(byte, 4, 4);   IHL = lq_bits_extract(byte, 0, 4); */
uint32_t lq_bits_extract(uint32_t value, unsigned shift, unsigned width);

/* ---- Text -------------------------------------------------------------------
 * Write the lowest `width` bits of `value` as '0'/'1' chars into `out`, with
 * a '_' every 4 bits for readability:  0x2A, width 8 -> "0010_1010".
 * Returns false (and writes nothing) if `out` is too small. Never overflows.
 * DECISION: the caller passes the buffer AND its size. Every C function that
 * writes into a buffer without being told its size is a future CVE. */
bool lq_to_binary(uint64_t value, unsigned width, char *out, size_t out_size);

/* ---- Arithmetic --------------------------------------------------------------
 * a + b, but stops at `cap` instead of overflowing. (Damage cap 9999, anyone?) */
uint32_t lq_sat_add_u32(uint32_t a, uint32_t b, uint32_t cap);

/* a - b, but stops at 0 instead of wrapping around to a huge number. */
uint32_t lq_sat_sub_u32(uint32_t a, uint32_t b);

/* ---- Memory -----------------------------------------------------------------
 * true if this machine stores the lowest byte of a number first in memory. */
bool lq_is_little_endian(void);

/* Print `len` bytes starting at `data` like a hex editor (HxD-style):
 *   offset | 16 hex bytes | ASCII
 * You'll reuse this in hexview, the save editor, and the binary parsers. */
void lq_hexdump(const void *data, size_t len, FILE *out);

#endif /* LQ_BITS_H */
