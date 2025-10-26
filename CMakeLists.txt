cmake_minimum_required(VERSION 3.28)

project(lexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(lexer 
	src/main.cpp 
	src/stream_parser.cpp	src/stream_parser.hpp 
	src/tokenize.cpp      	src/tokenize.hpp 
	src/token_list.cpp    	src/token_list.hpp 
	src/ast.cpp           	src/ast.hpp
	src/codegen.cpp
)

