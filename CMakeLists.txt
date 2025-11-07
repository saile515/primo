cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "primo")

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE source_files src/*.cpp)

add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME} LLVM)
