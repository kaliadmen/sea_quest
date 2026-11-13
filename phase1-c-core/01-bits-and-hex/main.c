/* =============================================================================
 * Lesson 01 — BITS & HEX LAB
 * =============================================================================
 * Everything in a computer is bits. Every exploit, every save file, every
 * network packet, every GameShark code you ever typed is someone deciding
 * what a pattern of bits MEANS. This lab lets you poke at that directly.
 *
 * HOW TO USE THIS FILE
 *   1. Read README.md first (10 min).
 *   2. Run it (CLion: pick "l01_bits_lab" top-right, press the green arrow).
 *   3. Open PREDICT.md. For each experiment: write your prediction, change the
 *      KNOB, re-run, compare. Being wrong is the point. That's where learning happens.
 *
 * Reading order inside this file: KNOBS -> main() at the bottom -> each lab.
 * ========================================================================== */

#include <limits.h>   /* INT_MAX, CHAR_BIT ... the limits of each type     */
#include <stddef.h>   /* offsetof(), size_t                                */
#include <stdint.h>   /* uint8_t, int16_t ... exact-width integer types     */
#include <stdio.h>    /* printf                                            */
#include <string.h>   /* memset, memcpy, strncpy                           */

#include "lq/bits.h"  /* YOUR library: bit helpers + hexdump               */
#include "lq/input.h" /* YOUR library: safe number input                   */

/* =============================================================================
 * KNOBS: change these, rebuild, re-run. PREDICT.md tells you which to try.
 * ========================================================================== */

/* The type used to store the hero's HP in the Overflow Arena (lab 3).
 * Try: uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t
 * (Stick to 8/16/32-bit types; the clamp math below assumes <= 32 bits.) */
#define KNOB_HP_TYPE        uint8_t

/* Starting HP in the arena. Values that don't fit KNOB_HP_TYPE get truncated
 * at startup. Watch what 300 becomes in a uint8_t. */
#define KNOB_START_HP       200

/* 0 = raw machine math (wraps around like an odometer)
 * 1 = saturating math (clamps at 0 and at the type's max). What games should do. */
#define KNOB_USE_SATURATION 0

/* Maximum damage per hit, Final Fantasy-style. */
#define KNOB_DAMAGE_CAP     9999

/* Hero struct layout for Memory Peek (lab 5):
 * 1 = fields in "natural" order (as a person would list them)
 * 2 = fields sorted largest-first (how a pro arranges them). Compare sizeof! */
#define KNOB_HERO_LAYOUT    1

/* Byte used to pre-fill the hero struct, so PADDING bytes stand out in the
 * hex dump. 0xCC is what MSVC fills uninitialized stack memory with in Debug
 * builds. If you ever see "cc cc cc cc" in x64dbg, that's why. */
#define KNOB_PADDING_FILL   0xCC

/* ========================================================================== */

/* Small helper: print a horizontal rule + title. `static` = private to this file. */
static void banner(const char *title) {
    printf("\n------------------------------------------------------------\n");
    printf("  %s\n", title);
    printf("------------------------------------------------------------\n");
}

/* =============================================================================
 * LAB 1 — Type sizes on THIS machine
 * -----------------------------------------------------------------------------
 * C only guarantees MINIMUM sizes (int >= 16 bits, long >= 32 bits...).
 * The real sizes depend on the platform's "data model":
 *   Windows 64-bit = LLP64: int 4, long 4, long long 8, pointer 8
 *   Linux 64-bit   = LP64 : int 4, long 8, long long 8, pointer 8
 * Run this lab on Windows AND on your Ubuntu VM and compare the `long` row.
 * ========================================================================== */
static void lab_type_sizes(void) {
    banner("LAB 1: Type sizes on THIS machine");

    /* sizeof is computed by the COMPILER, not at runtime. It's a constant.
     * Its result type is size_t, printed with %zu. */
    printf("  %-12s %5s   %s\n", "type", "bytes", "range");
    printf("  %-12s %5zu   %d .. %d\n", "char",      sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("  %-12s %5zu   %d .. %d\n", "short",     sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("  %-12s %5zu   %d .. %d\n", "int",       sizeof(int), INT_MIN, INT_MAX);
    printf("  %-12s %5zu   %ld .. %ld\n", "long",    sizeof(long), LONG_MIN, LONG_MAX);
    printf("  %-12s %5zu   %lld .. %lld\n", "long long", sizeof(long long), LLONG_MIN, LLONG_MAX);
    printf("  %-12s %5zu   0 .. %u\n", "unsigned",   sizeof(unsigned), UINT_MAX);
    printf("  %-12s %5zu\n", "float",  sizeof(float));
    printf("  %-12s %5zu\n", "double", sizeof(double));
    printf("  %-12s %5zu   <- 8 = 64-bit program, 4 = 32-bit\n", "void*", sizeof(void *));
    printf("  %-12s %5zu\n", "size_t", sizeof(size_t));

    printf("\n  Bits per byte (CHAR_BIT): %d\n", CHAR_BIT);
    printf("  Is `char` signed here?   %s\n", (CHAR_MIN < 0) ? "yes" : "no (ARM often says no!)");
    printf("  Byte order:              %s-endian\n", lq_is_little_endian() ? "little" : "big");

    printf("\n  TAKEAWAY: when exact size matters (files, packets, saves), use\n"
           "  uint8_t / uint16_t / uint32_t / uint64_t, never plain int or long.\n");
}

/* =============================================================================
 * LAB 2 — Number Inspector
 * -----------------------------------------------------------------------------
 * One value, many views. Same bits; meaning depends on the TYPE you read
 * them as. This is the core idea behind every hex editor and debugger.
 * ========================================================================== */
static void lab_number_inspector(void) {
    banner("LAB 2: Number Inspector");
    printf("  Enter numbers as decimal (42), hex (0x2A), or binary (0b101010).\n");

    long long n;
    if (!lq_prompt_ll("  number> ", LLONG_MIN, LLONG_MAX, &n)) return;

    /* Reinterpret as unsigned 64-bit for display. Converting a NEGATIVE signed
     * value to unsigned is well-defined in C: it wraps modulo 2^64. That's
     * how you see two's complement: -1 becomes 0xFFFFFFFFFFFFFFFF. */
    uint64_t u = (uint64_t)n;

    char bin[96];
    unsigned width = (u <= 0xFFFFFFFFu) ? 32 : 64;
    lq_to_binary(u, width, bin, sizeof bin);

    printf("\n  decimal : %lld\n", n);
    printf("  hex     : 0x%llX\n", (unsigned long long)u);
    printf("  binary  : %s\n", bin);
    printf("  1-bits  : %u (popcount of low 32 bits)\n", lq_popcount((uint32_t)u));

    /* TRUNCATION: squeezing a big value into a small type keeps only the low
     * bits. Unsigned results are well-defined (mod 2^N). Signed results for
     * out-of-range values are "implementation-defined" (every compiler you'll
     * use just keeps the low bits, two's complement style). */
    printf("\n  Same value stored in smaller types (low bits kept):\n");
    printf("    uint8_t : %u\n",  (unsigned)(uint8_t)u);
    printf("    int8_t  : %d\n",  (int)(int8_t)(uint8_t)u);
    printf("    uint16_t: %u\n",  (unsigned)(uint16_t)u);
    printf("    int16_t : %d\n",  (int)(int16_t)(uint16_t)u);
    printf("    uint32_t: %lu\n", (unsigned long)(uint32_t)u);
    printf("    int32_t : %ld\n", (long)(int32_t)(uint32_t)u);

    /* ENDIANNESS: how the bytes of the number actually sit in RAM. */
    printf("\n  The 8 bytes of this long long, as they sit in memory:\n");
    lq_hexdump(&n, sizeof n, stdout);
    printf("  On a little-endian CPU (x86, most ARM) the LOWEST byte comes first.\n"
           "  That's why 0x2A shows up as \"2a 00 00 00...\" not \"...00 2a\".\n"
           "  Network packets use BIG-endian. Remember this for Lesson 15.\n");
}

/* =============================================================================
 * LAB 3 — HP Overflow Arena
 * -----------------------------------------------------------------------------
 * Unsigned math in C wraps around: 0 - 1 = max value. In a game, that's the
 * classic "boss takes damage and suddenly has 65535 HP" bug. In security, it's
 * worse: a wrapped SIZE calculation leads to a too-small buffer and a heap
 * overflow. Same bug, different stakes.
 * ========================================================================== */
typedef KNOB_HP_TYPE hp_t;   /* typedef = a new name for an existing type */

/* Is hp_t signed? _Generic (C11) picks a value based on the TYPE of an
 * expression, at compile time. It's C's tiny version of templates.
 * (The classic trick `(hp_t)-1 < 0` also works, but -Wextra warns that the
 * comparison is "always false" for unsigned types, which is true and noisy.) */
#define HP_IS_SIGNED   _Generic((hp_t)0, int8_t: 1, int16_t: 1, int32_t: 1, default: 0)
#define HP_BITS        (sizeof(hp_t) * CHAR_BIT)
#define HP_MAX         (HP_IS_SIGNED ? ((1LL << (HP_BITS - 1)) - 1) : ((1LL << HP_BITS) - 1))
#define HP_MIN         (HP_IS_SIGNED ? -(1LL << (HP_BITS - 1)) : 0LL)

/* Apply a change (+heal / -damage) to HP using the method chosen by the knob. */
static hp_t apply_hp_change(hp_t hp, long long delta) {
    long long result = (long long)hp + delta;   /* done in a wide type: no UB here */
#if KNOB_USE_SATURATION
    /* Clamp to [0, HP_MAX]. HP below 0 makes no sense, so 0 is the floor
     * even for signed types. */
    if (result < 0)      result = 0;
    if (result > HP_MAX) result = HP_MAX;
#endif
    /* Without saturation, this cast is where wrapping happens: only the low
     * HP_BITS bits survive. This is exactly what a CPU register does. */
    return (hp_t)result;
}

static void print_hp(hp_t hp) {
    char bin[96];
    /* Show the bits actually stored. For a signed type holding -1, the cast to
     * unsigned long long sign-extends to 64 one-bits, so mask down to the
     * type's real width. (HP_BITS <= 32, so the shift is safe.) */
    unsigned long long raw = (unsigned long long)hp & ((1ULL << HP_BITS) - 1);
    lq_to_binary(raw, (unsigned)HP_BITS, bin, sizeof bin);
    printf("  HP = %-6lld  hex 0x%0*llX  bin %s\n",
           (long long)hp, (int)(sizeof(hp_t) * 2), raw, bin);
}

static void lab_overflow_arena(void) {
    banner("LAB 3: HP Overflow Arena");
    printf("  hp_t is %zu byte(s), %s. Range %lld .. %lld. Saturation: %s\n",
           sizeof(hp_t), HP_IS_SIGNED ? "signed" : "unsigned",
           HP_MIN, HP_MAX, KNOB_USE_SATURATION ? "ON" : "OFF");

    long long requested = KNOB_START_HP;
    hp_t hp = (hp_t)requested;          /* truncation happens HERE if it doesn't fit */
    if ((long long)hp != requested) {
        printf("  !! KNOB_START_HP (%lld) didn't fit and was truncated to %lld\n",
               requested, (long long)hp);
    }
    print_hp(hp);

    for (;;) {
        printf("\n  1) take damage  2) heal  3) drink 10 potions (+50 each)  0) back\n");
        long long choice;
        if (!lq_prompt_ll("  arena> ", 0, 3, &choice) || choice == 0) return;

        if (choice == 1) {
            long long dmg;
            if (!lq_prompt_ll("  damage (0-99999)> ", 0, 99999, &dmg)) return;
            if (dmg > KNOB_DAMAGE_CAP) {
                printf("  Damage capped: %lld -> %d\n", dmg, KNOB_DAMAGE_CAP);
                dmg = KNOB_DAMAGE_CAP;
            }
            hp = apply_hp_change(hp, -dmg);
        } else if (choice == 2) {
            long long heal;
            if (!lq_prompt_ll("  heal (0-99999)> ", 0, 99999, &heal)) return;
            hp = apply_hp_change(hp, heal);
        } else {
            for (int i = 1; i <= 10; i++) {
                hp = apply_hp_change(hp, 50);
                printf("  potion %2d -> ", i);
                print_hp(hp);
            }
            continue;
        }
        print_hp(hp);
        if ((long long)hp <= 0) printf("  The hero falls...\n");
    }
}

/* =============================================================================
 * LAB 4 — Status Effects as Bit Flags
 * -----------------------------------------------------------------------------
 * 8 yes/no facts packed into ONE byte. Games, file permissions (rwx), TCP
 * flags, Windows access masks, CPU registers: all of them do this.
 *   set:    flags |=  BIT       clear: flags &= ~BIT
 *   toggle: flags ^=  BIT       test:  (flags & BIT) != 0
 * ========================================================================== */
typedef uint8_t status_t;

enum {                        /* each name is one bit position */
    ST_POISON  = 1u << 0,     /* 0000_0001 */
    ST_SLEEP   = 1u << 1,     /* 0000_0010 */
    ST_SILENCE = 1u << 2,     /* 0000_0100 */
    ST_BLIND   = 1u << 3,     /* 0000_1000 */
    ST_HASTE   = 1u << 4,     /* 0001_0000 */
    ST_SLOW    = 1u << 5,     /* 0010_0000 */
    ST_PROTECT = 1u << 6,     /* 0100_0000 */
    ST_REGEN   = 1u << 7,     /* 1000_0000 */
};

/* A MASK is a group of bits treated together. Esuna cures all of these at once. */
#define ST_NEGATIVE_MASK (ST_POISON | ST_SLEEP | ST_SILENCE | ST_BLIND | ST_SLOW)

static const char *const STATUS_NAMES[8] = {
    "Poison", "Sleep", "Silence", "Blind", "Haste", "Slow", "Protect", "Regen"
};

static void print_status(status_t s) {
    char bin[16];
    lq_to_binary(s, 8, bin, sizeof bin);
    printf("  status = 0x%02X  %s  [", (unsigned)s, bin);
    int any = 0;
    for (unsigned i = 0; i < 8; i++) {
        if (lq_bit_test(s, i)) { printf("%s%s", any ? " " : "", STATUS_NAMES[i]); any = 1; }
    }
    printf("%s]\n", any ? "" : "healthy");
}

static void lab_status_flags(void) {
    banner("LAB 4: Status Effects (bit flags)");
    status_t s = 0;
    print_status(s);

    for (;;) {
        printf("\n  Bits: 0 Poison 1 Sleep 2 Silence 3 Blind 4 Haste 5 Slow 6 Protect 7 Regen\n");
        printf("  1) toggle a bit  2) cast Esuna (cure negatives)  3) cast Haste  4) set raw byte  0) back\n");
        long long choice;
        if (!lq_prompt_ll("  status> ", 0, 4, &choice) || choice == 0) return;

        if (choice == 1) {
            long long bit;
            if (!lq_prompt_ll("  bit (0-7)> ", 0, 7, &bit)) return;
            s = (status_t)lq_bit_toggle(s, (unsigned)bit);
        } else if (choice == 2) {
            s = (status_t)(s & ~ST_NEGATIVE_MASK);
            printf("  Esuna: flags &= ~0x%02X\n", (unsigned)ST_NEGATIVE_MASK);
        } else if (choice == 3) {
            /* Game RULE built from bit ops: Haste removes Slow. */
            s = (status_t)((s | ST_HASTE) & ~ST_SLOW);
            printf("  Haste: flags = (flags | 0x%02X) & ~0x%02X\n",
                   (unsigned)ST_HASTE, (unsigned)ST_SLOW);
        } else {
            long long raw;
            if (!lq_prompt_ll("  raw byte (0-255, try 0xFF or 0b1010)> ", 0, 255, &raw)) return;
            s = (status_t)raw;
        }
        print_status(s);
    }
}

/* =============================================================================
 * LAB 5 — Memory Peek (GameShark mode)
 * -----------------------------------------------------------------------------
 * A GameShark code is literally "at address X, write byte Y". Here you do that
 * to your OWN program: dump a struct's raw bytes, poke one, see the stat change.
 * You'll also discover PADDING: invisible bytes the compiler inserts so each
 * field starts at an address the CPU likes (a 4-byte field at a multiple of 4).
 * ========================================================================== */
#if KNOB_HERO_LAYOUT == 1
typedef struct {              /* natural order: how you'd describe a hero */
    char     name[10];
    uint8_t  level;
    uint16_t hp;
    uint16_t mp;
    uint32_t gil;
    uint8_t  status;
} Hero;
#else
typedef struct {              /* largest fields first: less padding */
    uint32_t gil;
    uint16_t hp;
    uint16_t mp;
    char     name[10];
    uint8_t  level;
    uint8_t  status;
} Hero;
#endif

static void print_hero_fields(const Hero *h) {
    /* offsetof(Type, field) = how many bytes from the start of the struct. */
    printf("  field    offset size  value\n");
    printf("  name     %4zu   %3zu   \"%.*s\"\n", offsetof(Hero, name), sizeof h->name,
           (int)sizeof h->name, h->name);
    printf("  level    %4zu   %3zu   %u\n", offsetof(Hero, level), sizeof h->level, (unsigned)h->level);
    printf("  hp       %4zu   %3zu   %u\n", offsetof(Hero, hp), sizeof h->hp, (unsigned)h->hp);
    printf("  mp       %4zu   %3zu   %u\n", offsetof(Hero, mp), sizeof h->mp, (unsigned)h->mp);
    printf("  gil      %4zu   %3zu   %lu\n", offsetof(Hero, gil), sizeof h->gil, (unsigned long)h->gil);
    printf("  status   %4zu   %3zu   0x%02X\n", offsetof(Hero, status), sizeof h->status, (unsigned)h->status);

    size_t used = sizeof h->name + sizeof h->level + sizeof h->hp + sizeof h->mp
                + sizeof h->gil + sizeof h->status;
    printf("  sizeof(Hero) = %zu bytes, fields use %zu -> %zu bytes of padding\n",
           sizeof *h, used, sizeof *h - used);
}

static void lab_memory_peek(void) {
    banner("LAB 5: Memory Peek (GameShark mode)");

    Hero hero;
    memset(&hero, KNOB_PADDING_FILL, sizeof hero);   /* make padding visible */
    memset(hero.name, 0, sizeof hero.name);
    memcpy(hero.name, "Cecil", 5);
    hero.level  = 10;
    hero.hp     = 600;        /* 0x0258 -> stored as bytes 58 02 (little-endian!) */
    hero.mp     = 40;
    hero.gil    = 1500;
    hero.status = 0;

    for (;;) {
        printf("\n");
        print_hero_fields(&hero);
        printf("\n");
        lq_hexdump(&hero, sizeof hero, stdout);

        printf("\n  1) poke a byte  0) back\n");
        long long choice;
        if (!lq_prompt_ll("  peek> ", 0, 1, &choice) || choice == 0) return;

        long long off, val;
        /* BOUNDS CHECK: the prompt only allows offsets INSIDE the struct.
         * Without this, a "poke" could write past the end of `hero` into
         * whatever is next on the stack. That's an out-of-bounds write, the
         * root of many real exploits. See EXERCISES.md task 4. */
        if (!lq_prompt_ll("  offset> ", 0, (long long)sizeof hero - 1, &off)) return;
        if (!lq_prompt_ll("  new byte value (0-255)> ", 0, 255, &val)) return;

        unsigned char *bytes = (unsigned char *)&hero;   /* view the struct as raw bytes */
        printf("  poke [%lld]: 0x%02X -> 0x%02X\n", off, (unsigned)bytes[off], (unsigned)val);
        bytes[off] = (unsigned char)val;
    }
}

/* =============================================================================
 * LAB 6 — Network Field Decoder
 * -----------------------------------------------------------------------------
 * Same shift-and-mask skills, security context. Wireshark does exactly this
 * for every field of every packet. You'll write a full parser in Lesson 15.
 * ========================================================================== */
static void lab_network_decoder(void) {
    banner("LAB 6: Network Field Decoder");

    printf("  IPv4 header byte 0 packs two 4-bit fields: [version | header length].\n");
    printf("  A normal IPv4 packet starts with 0x45. Try it, then try 0x46 and 0x60.\n");
    long long b;
    if (!lq_prompt_ll("  IPv4 byte 0> ", 0, 255, &b)) return;
    uint32_t version = lq_bits_extract((uint32_t)b, 4, 4);    /* high nibble */
    uint32_t ihl     = lq_bits_extract((uint32_t)b, 0, 4);    /* low nibble  */
    printf("  version = %lu, IHL = %lu -> header is %lu bytes%s\n",
           (unsigned long)version, (unsigned long)ihl, (unsigned long)(ihl * 4),
           ihl > 5 ? " (has IP options: unusual, worth a look)" : "");
    if (version != 4) printf("  Not IPv4! (6 would mean IPv6)\n");
    if (ihl < 5)      printf("  IHL < 5 is INVALID: a parser that trusts this can misread memory.\n");

    printf("\n  TCP flags byte: bit0 FIN, 1 SYN, 2 RST, 3 PSH, 4 ACK, 5 URG.\n");
    printf("  Try 0x02 (SYN), 0x12 (SYN+ACK), 0x29 (FIN+URG+PSH, the 'Xmas' scan).\n");
    if (!lq_prompt_ll("  TCP flags> ", 0, 255, &b)) return;
    static const char *const TCP_FLAGS[6] = { "FIN", "SYN", "RST", "PSH", "ACK", "URG" };
    printf("  flags:");
    for (unsigned i = 0; i < 6; i++) {
        if (lq_bit_test((uint32_t)b, i)) printf(" %s", TCP_FLAGS[i]);
    }
    printf("\n");

    /* A SOC analyst reads these combos like words. */
    uint32_t f = (uint32_t)b & 0x3Fu;
    if (f == 0x02)      printf("  -> SYN only: someone is opening a connection (or scanning).\n");
    else if (f == 0x12) printf("  -> SYN+ACK: the server said yes.\n");
    else if (f == 0x00) printf("  -> NULL scan: no flags at all is never normal traffic.\n");
    else if (f == 0x29) printf("  -> Xmas scan: FIN+PSH+URG together is a scanner fingerprint.\n");
    else if ((f & 0x03) == 0x03) printf("  -> SYN+FIN together is invalid. A classic evasion attempt.\n");
}

/* =============================================================================
 * main: the menu loop
 * ========================================================================== */
int main(void) {
    printf("============================================================\n");
    printf("  SEA QUEST, Lesson 01: BITS & HEX LAB\n");
    printf("  Numbers accept decimal, 0x hex, or 0b binary everywhere.\n");
    printf("============================================================\n");

    for (;;) {
        printf("\n  1) Type sizes on this machine\n"
               "  2) Number inspector\n"
               "  3) HP overflow arena\n"
               "  4) Status effects (bit flags)\n"
               "  5) Memory peek (GameShark mode)\n"
               "  6) Network field decoder\n"
               "  0) Quit\n");
        long long choice;
        if (!lq_prompt_ll("> ", 0, 6, &choice) || choice == 0) break;

        switch (choice) {
            case 1: lab_type_sizes();       break;
            case 2: lab_number_inspector(); break;
            case 3: lab_overflow_arena();   break;
            case 4: lab_status_flags();     break;
            case 5: lab_memory_peek();      break;
            case 6: lab_network_decoder();  break;
            default: break;   /* unreachable: the prompt enforces 0..6 */
        }
    }
    printf("Bye! Now go fill in PREDICT.md and QUIZ.md.\n");
    return 0;
}
