# =============================================================================
# Sanitizers.cmake — runtime bug detectors
# =============================================================================
# A sanitizer adds checks to your program at compile time. When the program
# does something illegal at runtime (reads past an array, uses freed memory,
# overflows a signed int), it stops and prints EXACTLY where and why.
#
#   ASan  = AddressSanitizer        -> out-of-bounds, use-after-free, leaks
#   UBSan = UndefinedBehaviorSanitizer -> signed overflow, bad shifts, misaligned ptrs
#
# Support matrix (this matters on your multiple machines):
#   GCC/Clang on Ubuntu : ASan + UBSan   <- best place to hunt bugs
#   MSVC on Windows     : ASan only
#   MinGW on Windows    : neither (CLion's bundled toolchain) -> we skip quietly
#
# DECISION: sanitizers ON by default in Debug builds. They slow programs down
# ~2x, which is irrelevant for lessons and priceless for learning.
# Turn off with:  -DSQ_SANITIZE=OFF   (CLion: Settings > Build > CMake > options)
# =============================================================================

option(SQ_SANITIZE "Enable ASan/UBSan in Debug builds where supported" ON)

function(sq_sanitize target)
    if(NOT SQ_SANITIZE)
        return()
    endif()
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CMAKE_CONFIGURATION_TYPES)
        return()
    endif()

    if(MSVC)
        target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
        target_link_options(${target} PRIVATE /INCREMENTAL:NO)
    elseif(MINGW)
        # MinGW doesn't ship sanitizer runtimes. Use the Ubuntu VM for bug hunts.
        return()
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        set(_flags -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined)
        target_compile_options(${target} PRIVATE ${_flags} -g)
        target_link_options(${target} PRIVATE ${_flags})
    endif()
endfunction()

# Convenience: strict warnings + sanitizers in one call.
function(sq_lesson_defaults target)
    sq_strict_warnings(${target})
    sq_sanitize(${target})
endfunction()
