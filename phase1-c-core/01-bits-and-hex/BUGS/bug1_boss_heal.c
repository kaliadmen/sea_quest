/* =============================================================================
 * BUG 1 — "The boss that wouldn't die"
 * =============================================================================
 * SYMPTOM: The party hits the boss for 300. The boss had 250 HP left.
 *          Instead of dying, the boss now has a HUGE amount of HP.
 *
 * YOUR JOB:
 *   1. Run l01_bug1. Confirm the symptom.
 *   2. Before changing anything, write in NOTES.md WHY you think it happens.
 *   3. Set a breakpoint on the marked line (click the gutter in CLion),
 *      debug, and watch `boss_hp` before and after.
 *   4. Fix it TWO ways: (a) check before subtracting, (b) use lq_sat_sub_u32.
 *   5. Bonus: the compiler did NOT warn about this line, even with -Wconversion.
 *      Why not? Hint: look up "integer promotion". What TYPE is
 *      `boss_hp - damage` before it gets stored back? What VALUE is it?
 *      Lesson: warnings catch a lot, but not everything. You still have to think.
 *
 * Don't open SOLUTIONS.md until you've tried for at least 15 minutes.
 * ========================================================================== */
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint16_t boss_hp = 250;
    uint16_t damage  = 300;

    printf("Boss HP: %u. Party attacks for %u!\n", (unsigned)boss_hp, (unsigned)damage);

    boss_hp = boss_hp - damage;            /* <-- breakpoint here */

    if (boss_hp == 0) {
        printf("Boss defeated!\n");
    } else {
        printf("Boss HP is now %u. The boss laughs at you.\n", (unsigned)boss_hp);
    }
    return 0;
}
