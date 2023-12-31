cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
project(huffman)

# NOTE: huffman functionality should be implemented as a library and linked to the executables
# NOTE: tool target name must be "huffman-tool"

find_package(GTest REQUIRED)

add_library(huffman STATIC
        lib/reader.cpp
        lib/writer.cpp
        lib/huffman.cpp
        lib/huffman_tree.cpp
        lib/code.cpp
        lib/include/reader.h
)

add_subdirectory(unit-tests)

target_include_directories(huffman PUBLIC lib/include)

add_executable(huffman-tool huffman-tool.cpp)

if (MSVC)
    add_compile_options(/W4 /permissive-)
    if (TREAT_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif ()
else ()
    add_compile_options(-Wall -pedantic -Wextra -Wno-sign-compare)
    add_compile_options(-Wold-style-cast -Wextra-semi -Woverloaded-virtual -Wzero-as-null-pointer-constant)
    if (TREAT_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror -pedantic-errors)
    endif ()
endif ()

# Compiler specific warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wshadow=compatible-local)
    add_compile_options(-Wduplicated-branches)
    add_compile_options(-Wduplicated-cond)
    add_compile_options(-Wnull-dereference)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wshadow-uncaptured-local)
    add_compile_options(-Wloop-analysis)
    add_compile_options(-Wno-self-assign-overloaded)
endif ()

option(USE_SANITIZERS "Enable to build with undefined,leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    message(STATUS "Enabling sanitizers...")
    add_compile_options(-fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    add_link_options(-fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling libc++...")
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "Enabling _GLIBCXX_DEBUG...")
    add_compile_options(-D_GLIBCXX_DEBUG)
endif ()

target_link_libraries(huffman-tool huffman)
