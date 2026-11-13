# =============================================================================
# Warnings.cmake — turn the compiler into a strict code reviewer
# =============================================================================
# DECISION: maximum warnings from day one. Most C security bugs (integer
# truncation, signed/unsigned mixups, shadowed variables) are things the
# compiler can *already warn you about* if you ask. Security pros build with
# these on. Learning with them on builds the habit.
#
# Usage in a lesson's CMakeLists:   sq_strict_warnings(my_target)
# =============================================================================

option(SQ_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

function(sq_strict_warnings target)
    if(MSVC)
        # /W4 = high warning level. /permissive- = strict standards mode.
        target_compile_options(${target} PRIVATE /W4 /permissive-)
        if(SQ_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        # GCC and Clang (Ubuntu, and MinGW on Windows)
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wconversion        # implicit narrowing: int -> uint8_t etc.
            -Wsign-conversion   # signed <-> unsigned surprises
            -Wshadow            # inner variable hides an outer one
            -Wformat=2          # printf format string problems
            -Wcast-qual
            -Wnull-dereference
        )
        if(SQ_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()
