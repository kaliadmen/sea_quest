/* Tests for lq/bits.h. Run: ctest --test-dir build   (or the test_bits target in CLion)
 *
 * TESTING HABIT: test the edges, not just the middle. Bit 0, bit 31, bit 32
 * (invalid), width 32, a cap exactly reached, an add that would overflow.
 * Bugs live at the edges. Attackers look there first. */
#include "lq/bits.h"
#include "lq/qtest.h"

static void test_single_bits(void) {
    QT_CHECK(lq_bit_test(0x1u, 0));
    QT_CHECK(!lq_bit_test(0x1u, 1));
    QT_CHECK(lq_bit_test(0x80000000u, 31));
    QT_CHECK(!lq_bit_test(0xFFFFFFFFu, 32));          /* out of range -> false, not UB */

    QT_EQ_INT(lq_bit_set(0u, 3), 0x8);
    QT_EQ_INT(lq_bit_clear(0xFFu, 0), 0xFE);
    QT_EQ_INT(lq_bit_toggle(lq_bit_toggle(0x5u, 2), 2), 0x5); /* toggle twice = no change */
    QT_EQ_INT(lq_bit_set(7u, 40), 7);                  /* invalid bit -> unchanged */
}

static void test_popcount(void) {
    QT_EQ_INT(lq_popcount(0u), 0);
    QT_EQ_INT(lq_popcount(0xFFFFFFFFu), 32);
    QT_EQ_INT(lq_popcount(0x2Au), 3);                  /* 0b101010 */
}

static void test_extract(void) {
    /* IPv4 header byte 0x45: version 4, header length 5 (x 4 bytes = 20). */
    QT_EQ_INT(lq_bits_extract(0x45u, 4, 4), 4);
    QT_EQ_INT(lq_bits_extract(0x45u, 0, 4), 5);
    QT_EQ_INT(lq_bits_extract(0xDEADBEEFu, 0, 32), 0xDEADBEEF);
    QT_EQ_INT(lq_bits_extract(0xDEADBEEFu, 28, 8), 0xD);   /* width clipped */
    QT_EQ_INT(lq_bits_extract(1u, 32, 1), 0);
}

static void test_to_binary(void) {
    char buf[96];
    QT_CHECK(lq_to_binary(0x2A, 8, buf, sizeof buf));
    QT_EQ_STR(buf, "0010_1010");
    QT_CHECK(lq_to_binary(5, 3, buf, sizeof buf));
    QT_EQ_STR(buf, "101");

    char tiny[4];
    QT_CHECK(!lq_to_binary(0xFF, 8, tiny, sizeof tiny));   /* refuses, no overflow */
}

static void test_saturating(void) {
    QT_EQ_INT(lq_sat_add_u32(9000, 500, 9999), 9500);
    QT_EQ_INT(lq_sat_add_u32(9000, 5000, 9999), 9999);
    QT_EQ_INT(lq_sat_add_u32(UINT32_MAX - 1, 10, UINT32_MAX), UINT32_MAX); /* no wrap */
    QT_EQ_INT(lq_sat_sub_u32(50, 60), 0);
    QT_EQ_INT(lq_sat_sub_u32(60, 50), 10);
}

static void test_hexdump_smoke(void) {
    /* "Smoke test": just make sure it runs without crashing (ASan watches). */
    const char msg[] = "sea_quest!";
    lq_hexdump(msg, sizeof msg, stdout);
    lq_hexdump(msg, 0, stdout);
}

/* EXERCISE (Lesson 01, task 5): add lq_rotl32() to bits.h / bits.c, then
 * change `#if 0` to `#if 1` and make these pass. */
#if 0
static void test_rotl(void) {
    QT_EQ_INT(lq_rotl32(0x80000001u, 1), 0x00000003);
    QT_EQ_INT(lq_rotl32(0x12345678u, 8), 0x34567812);
    QT_EQ_INT(lq_rotl32(0xABCDu, 0), 0xABCD);
    QT_EQ_INT(lq_rotl32(0xABCDu, 32), 0xABCD);   /* careful: no UB allowed! */
}
#endif

int main(void) {
    QT_RUN(test_single_bits);
    QT_RUN(test_popcount);
    QT_RUN(test_extract);
    QT_RUN(test_to_binary);
    QT_RUN(test_saturating);
    QT_RUN(test_hexdump_smoke);
#if 0
    QT_RUN(test_rotl);
#endif
    return QT_SUMMARY();
}
