
cmake_minimum_required(VERSION 3.20)

project(ConLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CONLAB_ENABLE_ASAN "Enable AddressSanitizer" ON)

find_package(Threads REQUIRED)

# Shared project configuration
add_library(conlab_options INTERFACE)

target_compile_options(conlab_options INTERFACE
        -Wall
        -Wextra
        -Wpedantic
)

if(CONLAB_ENABLE_ASAN AND APPLE)
    target_compile_options(conlab_options INTERFACE
            -fsanitize=address
            -fno-omit-frame-pointer
    )

    target_link_options(conlab_options INTERFACE
            -fsanitize=address
    )
endif()

# Main application
add_executable(conlab
        src/main.cpp
        src/sequential_queue.cpp
        src/mutex_queue.cpp
        src/atomic_counter.cpp
)

target_include_directories(conlab PRIVATE include)

target_link_libraries(conlab PRIVATE
        conlab_options
        Threads::Threads
)

# Benchmark executable
add_executable(conlab_benchmark
        src/benchmark.cpp
        src/sequential_queue.cpp
        src/mutex_queue.cpp
)

target_include_directories(conlab_benchmark PRIVATE include)

target_link_libraries(conlab_benchmark PRIVATE
        conlab_options
        Threads::Threads
)

# Automated tests
enable_testing()

# SequentialQueue tests
add_executable(sequential_queue_test
        tests/sequential_queue_test.cpp
        src/sequential_queue.cpp
)

target_include_directories(sequential_queue_test PRIVATE include)

target_link_libraries(sequential_queue_test PRIVATE
        conlab_options
)

add_test(
        NAME SequentialQueueTests
        COMMAND sequential_queue_test
)

# MutexQueue tests
add_executable(mutex_queue_test
        tests/mutex_queue_test.cpp
        src/mutex_queue.cpp
)

target_include_directories(mutex_queue_test PRIVATE include)

target_link_libraries(mutex_queue_test PRIVATE
        conlab_options
        Threads::Threads
)

add_test(
        NAME MutexQueueTests
        COMMAND mutex_queue_test
)

# AtomicCounter tests
add_executable(atomic_counter_test
        tests/atomic_counter_test.cpp
        src/atomic_counter.cpp
)

target_include_directories(atomic_counter_test PRIVATE include)

target_link_libraries(atomic_counter_test PRIVATE
        conlab_options
        Threads::Threads
)

add_test(
        NAME AtomicCounterTests
        COMMAND atomic_counter_test
)
