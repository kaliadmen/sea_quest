/* =============================================================================
 * input.c — implementation of lq/input.h
 * ========================================================================== */
#include "lq/input.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

bool lq_read_line(char *buf, size_t size, FILE *in) {
    if (buf == NULL || size < 2) return false;
    /* fgets takes an int size. A cast is fine here: our buffers are small. */
    if (fgets(buf, (int)(size > 4096 ? 4096 : size), in) == NULL) return false;

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';                 /* normal case: strip newline */
        if (len > 1 && buf[len - 2] == '\r') buf[len - 2] = '\0'; /* Windows CRLF */
    } else {
        /* No newline = line was too long (or the file ended without one).
         * Eat the rest of the line so it doesn't become the NEXT input. */
        int c;
        while ((c = fgetc(in)) != '\n' && c != EOF) { /* discard */ }
    }
    return true;
}

bool lq_parse_ll(const char *text, int base, long long *out) {
    if (text == NULL || out == NULL) return false;

    /* Skip leading spaces so " 42" works. */
    while (isspace((unsigned char)*text)) text++;
    /* ^ the (unsigned char) cast is required: passing a negative char to
     *   isspace() is undefined behavior. A tiny, very common C bug. */

    if (*text == '\0') return false;

    /* strtoll doesn't understand "0b" prefixes, so handle binary ourselves. */
    int use_base = base;
    bool negative = false;
    const char *p = text;
    if (*p == '-' || *p == '+') { negative = (*p == '-'); p++; }
    if ((base == 0 || base == 2) && p[0] == '0' && (p[1] == 'b' || p[1] == 'B')) {
        use_base = 2;
        p += 2;
        if (*p != '0' && *p != '1') return false;  /* "0b", "0b-1", "0b2" are junk */
    } else {
        p = text;          /* let strtoll handle sign + 0x itself */
        negative = false;
    }

    char *end = NULL;
    errno = 0;                                   /* strtoll reports overflow via errno */
    long long value = strtoll(p, &end, use_base);

    if (end == p) return false;                  /* no digits at all */
    if (errno == ERANGE) return false;           /* too big / too small */
    while (isspace((unsigned char)*end)) end++;  /* allow trailing spaces */
    if (*end != '\0') return false;              /* junk after the number: "12abc" */

    *out = negative ? -value : value;
    return true;
}

bool lq_prompt_ll(const char *prompt, long long min, long long max, long long *out) {
    char line[128];
    for (;;) {
        fputs(prompt, stdout);
        fflush(stdout);              /* make sure the prompt shows before we wait */
        if (!lq_read_line(line, sizeof line, stdin)) return false;

        long long v;
        if (!lq_parse_ll(line, 0, &v)) {
            printf("  '%s' isn't a number. Try 42, 0x2A, or 0b101010.\n", line);
            continue;
        }
        if (v < min || v > max) {
            printf("  %lld is out of range [%lld, %lld].\n", v, min, max);
            continue;
        }
        *out = v;
        return true;
    }
}
