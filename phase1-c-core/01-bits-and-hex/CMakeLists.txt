# Lesson 01 — Bits & Hex Lab
#
# Targets you'll see in CLion's run dropdown:
#   l01_bits_lab        the interactive lab (start here)
#   l01_bug1 .. bug3    planted-bug programs for the BUGS/ drill

add_executable(l01_bits_lab main.c)
target_link_libraries(l01_bits_lab PRIVATE libquest)
sq_lesson_defaults(l01_bits_lab)

foreach(n 1 2 3)
    file(GLOB _bug_src ${CMAKE_CURRENT_SOURCE_DIR}/BUGS/bug${n}_*.c)
    add_executable(l01_bug${n} ${_bug_src})
    target_link_libraries(l01_bug${n} PRIVATE libquest)
    # Bug programs get the same strict warnings + sanitizers as everything else.
    # Reading the build output is part of the drill: at least one of these bugs
    # is announced by the compiler before you ever run it.
    sq_lesson_defaults(l01_bug${n})
endforeach()
