/* =============================================================================
 * BUG 3 — "Opening an empty treasure chest crashes the game"
 * =============================================================================
 * SYMPTOM: Printing a chest with items works. Printing an EMPTY chest
 *          crashes (on Ubuntu with sanitizers, you get a big red ASan report).
 *
 * YOUR JOB:
 *   1. Run l01_bug3 ON THE UBUNTU VM (Debug build) and read the ASan report
 *      from the top. It tells you: what kind of bug, which line, and which
 *      variable's memory was touched. Learning to read these reports is a
 *      core skill. Write down the three things it told you in NOTES.md.
 *   2. Find the line. Work out the value of `count - 1` when count == 0 and
 *      count is a size_t. (Lab 3 in the main program is the hint.)
 *   3. Fix the loop so it works for 0, 1, and many items.
 *
 * WHY THIS ONE MATTERS: "size minus one" underflowing to a giant number is a
 * real pattern behind memory-safety vulnerabilities. Here it's a crash in
 * your own program. In the wild, the same mistake can let attacker-controlled
 * input read or write memory it shouldn't.
 *
 * (On Windows/MinGW without sanitizers it may print garbage or crash randomly.
 *  "Works on my machine" is how memory bugs hide.)
 * ========================================================================== */
#include <stddef.h>
#include <stdio.h>

static void print_chest(const char *const items[], size_t count) {
    printf("Chest contains %zu item(s):\n", count);
    for (size_t i = 0; i <= count - 1; i++) {
        printf("  - %s\n", items[i]);
    }
}

int main(void) {
    const char *const full_chest[] = { "Potion", "Phoenix Down", "Ether" };
    const char *const empty_chest[1] = { "(unused slot)" };

    print_chest(full_chest, 3);
    print_chest(empty_chest, 0);          /* an empty chest */
    return 0;
}
