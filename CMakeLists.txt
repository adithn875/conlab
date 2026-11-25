cmake_minimum_required(VERSION 3.20)

project(ConLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(conlab
        src/main.cpp
        src/sequential_queue.cpp
)

target_include_directories(conlab PRIVATE include)

if(APPLE)
    target_compile_options(conlab PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(conlab PRIVATE -fsanitize=address)
endif()