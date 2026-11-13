/* =============================================================================
 * BUG 2 — "The poisoned hero who fell asleep"
 * =============================================================================
 * SYMPTOM: The hero is ONLY poisoned. The game says the hero is asleep, skips
 *          their turn, and prints "Zzz...". Two checks, both wrong.
 *
 * YOUR JOB:
 *   1. Run l01_bug2. Then try status = 0 and status = ST_SLEEP. Make a little
 *      table in NOTES.md: input -> what each check printed -> what it SHOULD print.
 *   2. Read the build warnings. GCC/Clang flag this one.
 *   3. Work out by hand what each condition ACTUALLY computes. (Hint: look up
 *      "C operator precedence" and find where == and != sit compared to &.)
 *   4. Fix both. Then write a rule for yourself in NOTES.md that prevents it forever.
 *
 * Notice the second check isn't random: it tests the WRONG BIT. Bugs that are
 * "almost right" are the dangerous ones, because they pass casual testing.
 * ========================================================================== */
#include <stdint.h>
#include <stdio.h>

#define ST_POISON 0x01u
#define ST_SLEEP  0x02u

int main(void) {
    uint8_t status = ST_POISON;        /* poisoned, wide awake */

    if (status & ST_SLEEP == 0) {
        printf("Hero can act.\n");
    } else {
        printf("Hero is asleep. Turn skipped.\n");
    }

    if (status & ST_SLEEP != 0) {
        printf("Zzz...\n");
    }
    return 0;
}
