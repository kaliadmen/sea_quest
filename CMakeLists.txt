# =============================================================================
# sea_quest — root build file
# =============================================================================
# CMake is a *build system generator*: it reads this file and writes the real
# build files for whatever compiler you're on (Ninja/Make on Ubuntu, MSVC or
# MinGW on Windows). That's why one repo builds on all your machines.
#
# CLion reads this file automatically. Open the repo folder -> CLion "loads the
# CMake project" -> every lesson shows up as a run target in the top-right.
#
# DECISION: one root CMakeLists that pulls in every lesson with add_subdirectory()
# instead of a separate project per lesson. Why: you open ONE project in CLion,
# libquest (your library) is shared by everything, and CI builds it all at once.
# =============================================================================
cmake_minimum_required(VERSION 3.20)

project(sea_quest
        VERSION 0.1.0
        DESCRIPTION "Learning C/C++ for security and games"
        LANGUAGES C)        # CXX gets added when we hit Phase 4 (C++).

# --- Language standard --------------------------------------------------------
# C17 is the current mainstream standard. "REQUIRED" = fail loudly if the
# compiler can't do it. "EXTENSIONS OFF" = no GNU-only extras sneaking in, so
# code that compiles on Ubuntu also compiles with MSVC on Windows.
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Default to Debug if nobody picked a build type (CLion always picks one).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Lets CLion / clangd understand the code perfectly (writes compile_commands.json).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Shared settings: warnings + sanitizers -----------------------------------
include(cmake/Warnings.cmake)
include(cmake/Sanitizers.cmake)

# Testing: CTest runs every registered test. In CLion: right-click a test
# target, or run "All CTest". From a terminal: ctest --test-dir build
enable_testing()

# --- The code -----------------------------------------------------------------
add_subdirectory(libquest)          # your own growing C library
add_subdirectory(phase1-c-core)     # Phase 1 lessons
