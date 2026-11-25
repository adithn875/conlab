cmake_minimum_required(VERSION 3.20)

project(ConLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(conlab
        src/main.cpp
        src/sequential_queue.cpp
        src/mutex_queue.cpp
        src/atomic_counter.cpp
)

add_executable(conlab_benchmark
        src/benchmark.cpp
        src/sequential_queue.cpp
        src/mutex_queue.cpp
)

target_include_directories(conlab PRIVATE include)
target_include_directories(conlab_benchmark PRIVATE include)

if(APPLE)
    target_compile_options(conlab PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(conlab PRIVATE -fsanitize=address)

    target_compile_options(conlab_benchmark PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(conlab_benchmark PRIVATE -fsanitize=address)
endif()