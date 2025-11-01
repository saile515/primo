cmake_minimum_required(VERSION 3.28)

project(lexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE source_files src/*.cpp)

add_executable(lexer ${source_files})

target_link_libraries(lexer LLVM)
