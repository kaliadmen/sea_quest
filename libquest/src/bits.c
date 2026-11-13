/* =============================================================================
 * bits.c — implementation of lq/bits.h
 * ========================================================================== */
#include "lq/bits.h"

/* Every function guards `n < 32`. Why? In C, shifting a 32-bit value by 32 or
 * more is UNDEFINED BEHAVIOR (UB). UB means the language makes no promise at
 * all: different compilers/CPUs do different things, and optimizers assume it
 * never happens. x86 masks the shift count to 5 bits, so `1u << 32` often
 * acts like `1u << 0` = 1. That's a silent, platform-dependent bug.
 * Rule: a shift count must be < the bit width of the type. */
#define LQ_BITS 32u

bool lq_bit_test(uint32_t value, unsigned n) {
    if (n >= LQ_BITS) return false;
    return (value >> n) & 1u;      /* slide bit n down to position 0, mask it */
}

uint32_t lq_bit_set(uint32_t value, unsigned n) {
    if (n >= LQ_BITS) return value;
    return value | (1u << n);      /* OR with a mask that has only bit n on */
}

uint32_t lq_bit_clear(uint32_t value, unsigned n) {
    if (n >= LQ_BITS) return value;
    return value & ~(1u << n);     /* AND with a mask that has every bit EXCEPT n */
}

uint32_t lq_bit_toggle(uint32_t value, unsigned n) {
    if (n >= LQ_BITS) return value;
    return value ^ (1u << n);      /* XOR flips: 0^1=1, 1^1=0 */
}

unsigned lq_popcount(uint32_t value) {
    /* Kernighan's trick: `v & (v - 1)` deletes the lowest 1-bit.
     * So the loop runs once per 1-bit, not 32 times.
     * Try it on paper with v = 0b1100:  1100 & 1011 = 1000;  1000 & 0111 = 0000. */
    unsigned count = 0;
    while (value) {
        value &= value - 1;
        count++;
    }
    return count;
}

uint32_t lq_bits_extract(uint32_t value, unsigned shift, unsigned width) {
    if (width == 0 || shift >= LQ_BITS) return 0;
    if (width > LQ_BITS - shift) width = LQ_BITS - shift;
    /* Build a mask of `width` ones. Careful: (1u << 32) is UB, so width 32 is
     * handled separately. Edge cases like this are where real bugs live. */
    uint32_t mask = (width == LQ_BITS) ? UINT32_MAX : ((1u << width) - 1u);
    return (value >> shift) & mask;
}

bool lq_to_binary(uint64_t value, unsigned width, char *out, size_t out_size) {
    if (out == NULL || width == 0 || width > 64) return false;

    /* Space needed: `width` digits + one '_' between each group of 4 + '\0'. */
    size_t needed = width + (width - 1) / 4 + 1;
    if (out_size < needed) return false;

    size_t pos = 0;
    for (unsigned i = width; i-- > 0;) {          /* count DOWN: print MSB first */
        out[pos++] = ((value >> i) & 1u) ? '1' : '0';
        if (i != 0 && i % 4 == 0) out[pos++] = '_';
    }
    out[pos] = '\0';
    return true;
}

uint32_t lq_sat_add_u32(uint32_t a, uint32_t b, uint32_t cap) {
    /* DECISION: check BEFORE adding. `if (a + b > cap)` looks right but is
     * wrong: if a + b overflows 32 bits it wraps to a small number and sails
     * under the cap. Rearranging to `b > cap - a` never overflows. This exact
     * pattern ("check the size before the math") prevents a whole class of
     * real-world heap overflow vulnerabilities. */
    if (a >= cap) return cap;
    if (b > cap - a) return cap;
    return a + b;
}

uint32_t lq_sat_sub_u32(uint32_t a, uint32_t b) {
    return (b > a) ? 0u : a - b;
}

bool lq_is_little_endian(void) {
    /* Store the number 1 in a 2-byte variable, then look at its FIRST byte
     * through an unsigned char pointer. Reading any object's bytes through
     * `unsigned char *` is always allowed in C. That's how memory viewers work. */
    uint16_t probe = 1;
    const unsigned char *first_byte = (const unsigned char *)&probe;
    return *first_byte == 1;
}

void lq_hexdump(const void *data, size_t len, FILE *out) {
    const unsigned char *bytes = data;   /* void* converts implicitly in C */

    for (size_t row = 0; row < len; row += 16) {
        fprintf(out, "%08zx  ", row);    /* %zx = size_t in hex */

        for (size_t i = 0; i < 16; i++) {
            if (row + i < len) fprintf(out, "%02x ", bytes[row + i]);
            else               fprintf(out, "   ");
            if (i == 7) fputc(' ', out);
        }

        fputs(" |", out);
        for (size_t i = 0; i < 16 && row + i < len; i++) {
            unsigned char c = bytes[row + i];
            /* Printable ASCII is 0x20 (space) .. 0x7e (~). Everything else
             * becomes '.', exactly like HxD's right-hand column. */
            fputc((c >= 0x20 && c <= 0x7e) ? c : '.', out);
        }
        fputs("|\n", out);
    }
}
