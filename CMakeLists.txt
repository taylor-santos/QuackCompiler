# CMake script for building the Quack Compiler.
#
#
cmake_minimum_required(VERSION 3.12)
project(QuackCompiler)
set(CMAKE_CXX_STANDARD 14)

# Where are the reflex include and library files?
#
set(REFLEX_LIB "/usr/local/lib/libreflex.a")
set(REFLEX_INCLUDE "/usr/local/include/reflex")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Recipes for generating C++ source files from .lxx and .yxx files
#

add_custom_command(
        OUTPUT lex.yy.cpp lex.yy.h
        COMMAND reflex --bison-cc --bison-locations --header-file ${CMAKE_CURRENT_SOURCE_DIR}/src/quack.lxx
        COMMAND mv lex.* build/
        DEPENDS src/quack.lxx quack.tab.hxx
)
add_custom_command(
        OUTPUT quack.tab.cxx quack.tab.hxx location.hh position.hh stack.hh
        COMMAND bison -d ${CMAKE_CURRENT_SOURCE_DIR}/src/quack.yxx
        COMMAND mv quack.* *.hh build/
        DEPENDS src/quack.yxx
)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${REFLEX_INCLUDE}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


add_executable(qcc
        build/quack.tab.cxx build/lex.yy.cpp build/lex.yy.h

        src/main.cpp include/parser.h include/AST.h src/AST.cpp include/Messages.h src/Messages.cpp)

target_link_libraries(qcc ${REFLEX_LIB})
