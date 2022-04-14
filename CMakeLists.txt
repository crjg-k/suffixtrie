cmake_minimum_required(VERSION 3.19)
project(SuffixTrie)

set(CMAKE_CXX_STANDARD 14)

add_executable(SuffixTrie main.cpp SuffixTrie.h SuffixTrie.cpp)
add_executable(test test.cpp)