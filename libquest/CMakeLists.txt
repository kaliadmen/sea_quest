# =============================================================================
# libquest — YOUR library. It grows every lesson.
# =============================================================================
# Every reusable piece you build (bit helpers, safe input, later: dynamic
# arrays, hash tables, heaps, graphs...) lands here, with tests. Lessons and
# final projects link against it instead of copy-pasting code.
#
# DECISION: a STATIC library (.a on Linux, .lib on Windows). The code gets
# copied into each program at link time, so there's no DLL / .so to ship
# alongside your tools or your game. Simple to distribute = good for a toolkit.
# =============================================================================

add_library(libquest STATIC
    src/bits.c
    src/input.c
)

# PUBLIC include dir: anything that links libquest can write  #include "lq/bits.h"
target_include_directories(libquest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
sq_lesson_defaults(libquest)

# Sanitizer flags are PRIVATE to each target, but a sanitized static library
# needs the sanitizer runtime at the final link step. Propagating the link
# flags avoids "undefined reference to __asan_..." errors in lessons.
if(SQ_SANITIZE AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC AND NOT MINGW
   AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(libquest INTERFACE -fsanitize=address,undefined)
endif()

# --- Tests --------------------------------------------------------------------
# Each test is a tiny program. Exit code 0 = pass. CTest runs them all.
foreach(t bits input)
    add_executable(test_${t} tests/test_${t}.c)
    target_link_libraries(test_${t} PRIVATE libquest)
    sq_lesson_defaults(test_${t})
    add_test(NAME libquest.${t} COMMAND test_${t})
endforeach()
