/* =============================================================================
 * qtest.h — a whole test framework in ~60 lines
 * =============================================================================
 * Why write our own instead of using a big framework (Unity, CMocka, GoogleTest)?
 *   1. Zero setup on every machine. It's one header.
 *   2. You can read all of it, and it teaches macros, which you'll meet in
 *      every real C codebase (Linux kernel, OpenSSL, game engines).
 * In Phase 4 (C++) we'll compare it with a real framework.
 *
 * Usage:
 *   static void test_something(void) { QT_CHECK(1 + 1 == 2); }
 *   int main(void) { QT_RUN(test_something); return QT_SUMMARY(); }
 *
 * MACRO NOTE: a macro is text substitution done BEFORE compiling. That's how
 * QT_CHECK can print the exact expression you wrote (#cond turns the code
 * into a string) and the file/line it's on (__FILE__, __LINE__).
 * The do { ... } while (0) wrapper makes a multi-statement macro behave like
 * ONE statement, so it's safe after an `if` without braces.
 * ========================================================================== */
#ifndef LQ_QTEST_H   /* "include guard": stops this file being pasted in twice */
#define LQ_QTEST_H

#include <stdio.h>
#include <string.h>

/* `static` at file scope = private to each .c file that includes this.
 * Every test program gets its own counters. */
static int qt_checks_  = 0;
static int qt_failures_ = 0;

#define QT_CHECK(cond)                                                      \
    do {                                                                    \
        qt_checks_++;                                                       \
        if (!(cond)) {                                                      \
            qt_failures_++;                                                 \
            fprintf(stderr, "  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                   \
    } while (0)

/* Compare two integers and show both values on failure. Cast to long long so
 * any integer type prints correctly with one format string. */
#define QT_EQ_INT(actual, expected)                                         \
    do {                                                                    \
        long long a_ = (long long)(actual), e_ = (long long)(expected);     \
        qt_checks_++;                                                       \
        if (a_ != e_) {                                                     \
            qt_failures_++;                                                 \
            fprintf(stderr, "  FAIL %s:%d: %s == %lld, expected %lld\n",    \
                    __FILE__, __LINE__, #actual, a_, e_);                   \
        }                                                                   \
    } while (0)

#define QT_EQ_STR(actual, expected)                                         \
    do {                                                                    \
        const char *a_ = (actual), *e_ = (expected);                        \
        qt_checks_++;                                                       \
        if (strcmp(a_, e_) != 0) {                                          \
            qt_failures_++;                                                 \
            fprintf(stderr, "  FAIL %s:%d: %s == \"%s\", expected \"%s\"\n", \
                    __FILE__, __LINE__, #actual, a_, e_);                   \
        }                                                                   \
    } while (0)

#define QT_RUN(test_fn)                                                     \
    do {                                                                    \
        printf("- %s\n", #test_fn);                                         \
        test_fn();                                                          \
    } while (0)

/* Prints a summary and evaluates to the process exit code (0 = all passed). */
#define QT_SUMMARY()                                                        \
    (printf("%d checks, %d failed\n", qt_checks_, qt_failures_),            \
     qt_failures_ == 0 ? 0 : 1)

#endif /* LQ_QTEST_H */
