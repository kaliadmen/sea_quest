/* Tests for lq/input.h. We test the PARSER, not the keyboard. Separating
 * "get text" from "interpret text" is what makes this testable at all.
 * (Same idea later: your log analyzer parses lines, not files.) */
#include "lq/input.h"
#include "lq/qtest.h"

#include <limits.h>

static void test_parse_good(void) {
    long long v = 0;
    QT_CHECK(lq_parse_ll("42", 0, &v));        QT_EQ_INT(v, 42);
    QT_CHECK(lq_parse_ll("  -7 ", 0, &v));     QT_EQ_INT(v, -7);
    QT_CHECK(lq_parse_ll("0x2A", 0, &v));      QT_EQ_INT(v, 42);
    QT_CHECK(lq_parse_ll("0b101010", 0, &v));  QT_EQ_INT(v, 42);
    QT_CHECK(lq_parse_ll("-0b11", 0, &v));     QT_EQ_INT(v, -3);
    QT_CHECK(lq_parse_ll("ff", 16, &v));       QT_EQ_INT(v, 255);
}

static void test_parse_bad(void) {
    long long v = 1234;
    QT_CHECK(!lq_parse_ll("", 0, &v));
    QT_CHECK(!lq_parse_ll("   ", 0, &v));
    QT_CHECK(!lq_parse_ll("abc", 10, &v));
    QT_CHECK(!lq_parse_ll("12abc", 10, &v));
    QT_CHECK(!lq_parse_ll("0b", 0, &v));
    QT_CHECK(!lq_parse_ll("0b2", 0, &v));
    QT_CHECK(!lq_parse_ll("99999999999999999999999", 10, &v)); /* overflow caught */
    QT_EQ_INT(v, 1234);   /* failed parses must NOT touch the output */
}

static void test_limits(void) {
    long long v = 0;
    QT_CHECK(lq_parse_ll("9223372036854775807", 10, &v));
    QT_CHECK(v == LLONG_MAX);
}

int main(void) {
    QT_RUN(test_parse_good);
    QT_RUN(test_parse_bad);
    QT_RUN(test_limits);
    return QT_SUMMARY();
}
