# ConLab — Concurrent Data Structures Laboratory

**ConLab** is a C++17 systems programming project focused on implementing, testing, and benchmarking sequential and concurrent data structures from scratch.

The project explores memory management, object lifetime, synchronization, atomic operations, compare-and-swap, producer-consumer coordination, and performance measurement.

Rather than treating concurrency as a black box, ConLab investigates how these mechanisms work internally through implementation and experimentation.

**Project status:** Phases 1–6 implemented; Phase 7 documentation in progress.

---

## Table of Contents

- [Objectives](#objectives)
- [Technology Stack](#technology-stack)
- [Repository Structure](#repository-structure)
- [Implemented Components](#implemented-components)
- [Building and Running](#building-and-running)
- [Testing and Validation](#testing-and-validation)
- [Benchmarking](#benchmarking)
- [Engineering Concepts](#engineering-concepts)
- [Limitations](#limitations)
- [Development Roadmap](#development-roadmap)

---

## Objectives

The main objectives of ConLab are:

- Understand pointers, dynamic memory allocation, and object lifetime.
- Implement linked-list-based data structures.
- Explore RAII, destructors, and copy semantics.
- Understand thread safety and shared mutable state.
- Implement synchronization using mutexes and condition variables.
- Explore atomic operations and compare-and-swap.
- Build producer-consumer systems.
- Investigate concurrency correctness through stress testing.
- Measure and interpret the performance of different queue configurations.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Primary programming language |
| CMake | Build system |
| AppleClang | Compiler used for current experiments |
| CLion | Development environment |
| `std::thread` | Multithreading |
| `std::mutex` | Mutual exclusion |
| `std::condition_variable` | Thread coordination |
| `std::atomic` | Atomic operations |
| AddressSanitizer | Memory error detection in Debug builds |
| `std::chrono` | Performance measurement |

---

## Repository Structure

```text
conlab/
│
├── include/
│   ├── sequential_queue.hpp
│   ├── mutex_queue.hpp
│   └── atomic_counter.hpp
│
├── src/
│   ├── main.cpp
│   ├── sequential_queue.cpp
│   ├── mutex_queue.cpp
│   ├── atomic_counter.cpp
│   └── benchmark.cpp
│
├── docs/
│   └── benchmark-report.md
│
├── tests/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

### Directory responsibilities

**include/**

Contains the public interfaces for the queue implementations and atomic counters.

**src/**

Contains the implementation files, test harness, and benchmark executable source.

**docs/**

Contains engineering documentation and experimental reports.

**tests/**

Reserved for future independent automated test suites.

---

## Implemented Components

### 1. Sequential Queue

A linked-list-based FIFO queue implemented from scratch.

FIFO means First In, First Out: the first element inserted is the first element removed.

The implementation uses:

- Dynamically allocated nodes.
- Head and tail pointers.
- A size counter.
- Explicit memory management.
- Copy construction and copy assignment.

Implemented operations include:

| Operation | Description |
|---|---|
| `enqueue()` | Inserts an element at the back |
| `dequeue()` | Removes the front element |
| `front()` | Reads the front element |
| `is_empty()` | Checks whether the queue is empty |
| `size()` | Returns the number of elements |
| `display()` | Displays queue contents |

The implementation also handles empty-queue access and supports independent copies of queue contents.

**Complexity**

| Operation | Time complexity |
|---|---|
| Enqueue | O(1) |
| Dequeue | O(1) |
| Front | O(1) |
| Is empty | O(1) |
| Size | O(1) |
| Display | O(n) |

### 2. Mutex-Protected Concurrent Queue

A thread-safe FIFO queue implemented using a linked list and C++ synchronization primitives.

Technologies used:

- `std::mutex`
- `std::lock_guard`
- `std::unique_lock`
- `std::condition_variable`
- `std::thread`

Implemented operations:

| Operation | Description |
|---|---|
| `enqueue()` | Inserts an element safely |
| `wait_dequeue()` | Waits for an element and removes it |
| `close()` | Closes the queue and wakes waiting consumers |
| `size()` | Returns the current queue size |
| `is_empty()` | Checks whether the queue is empty |

The queue supports:

- Multiple producers.
- Multiple consumers.
- Waiting consumers when the queue is empty.
- Graceful shutdown.
- Draining pending tasks after closure.
- Rejection of new tasks after closure.

The queue uses mutex-based synchronization. It is not lock-free.

### 3. Atomic Counters and Compare-and-Swap

ConLab includes three counter implementations:

| Counter | Synchronization |
|---|---|
| `NormalCounter` | No synchronization |
| `MutexCounter` | `std::mutex` |
| `AtomicCounter` | `std::atomic<int>` |

The atomic counter explores:

- `fetch_add()`
- `load()`
- `compare_exchange_weak()`
- `compare_exchange_strong()`
- `memory_order_relaxed`
- Default sequentially consistent atomic operations

A CAS-based increment loop retries when a compare-and-swap operation fails.

The normal counter is intended for single-threaded use. Concurrent unsynchronized modifications would introduce a data race.

---

## Building and Running

### Prerequisites

- C++17-compatible compiler.
- CMake 3.20 or later.
- Git.

### Clone the repository

```bash
git clone https://github.com/adithn875/conlab.git
cd conlab
```

### Debug build

Configure:

```bash
cmake -S . -B cmake-build-debug
```

Build:

```bash
cmake --build cmake-build-debug
```

Run the main executable:

```bash
./cmake-build-debug/conlab
```

On Apple platforms, the Debug configuration enables AddressSanitizer through CMake.

### Release build

Configure:

```bash
cmake -S . -B cmake-build-release \
  -DCMAKE_BUILD_TYPE=Release
```

Build:

```bash
cmake --build cmake-build-release
```

Run:

```bash
./cmake-build-release/conlab
```

### Run the benchmark

```bash
./cmake-build-release/conlab_benchmark
```

The project provides two executable targets:

| Target | Purpose |
|---|---|
| `conlab` | Main implementation and test harness |
| `conlab_benchmark` | Queue performance experiments |

---

## Testing and Validation

ConLab uses implementation-specific test harnesses and concurrent stress tests.

### Sequential Queue

Verified scenarios include:

- Empty queue behaviour.
- Enqueue and dequeue operations.
- FIFO ordering.
- Front access.
- Queue size.
- Copy construction.
- Copy assignment.
- Independent queue copies.
- Memory cleanup.

### Concurrent Queue

Verified scenarios include:

- Single producer and single consumer.
- Multiple producers and consumers.
- Consumers waiting on an empty queue.
- Queue closure.
- Consumers draining pending tasks after closure.
- Rejection of enqueue operations after closure.

### Producer-Consumer Stress Testing

The implementation was tested using different configurations:

| Producers | Consumers | Tasks |
|---:|---:|---:|
| 1 | 1 | 20,000 |
| 2 | 4 | 40,000 |
| 8 | 2 | 160,000 |

Validation checks include:

- Missing task detection.
- Duplicate task detection.
- Invalid task detection.
- Failed enqueue detection.
- Final queue size.

Ten repeated stress-test runs were also performed with eight producers and four consumers, processing 1.6 million tasks in total.

All reported runs passed their validation checks.

### Atomic Counter

Tested operations include:

- Sequential increments.
- Concurrent mutex-protected increments.
- Concurrent atomic increments.
- CAS success and failure cases.
- CAS-based concurrent increments.
- Relaxed atomic increments.

The concurrent counter experiments used four threads with 100,000 increments per thread.

---

## Benchmarking

ConLab includes a dedicated benchmark executable to investigate queue performance.

### Benchmark configurations

| Configuration | Producers | Consumers |
|---|---:|---:|
| Sequential queue | 1 | 1 |
| Mutex queue | 1 | 1 |
| Mutex queue | 2 | 2 |
| Mutex queue | 4 | 4 |
| Mutex queue | 8 | 4 |

Each configuration processes 100,000 tasks per repetition and is executed 15 times.

### Experimental environment

| Property | Value |
|---|---|
| Operating system | macOS |
| Compiler | Apple LLVM 21.0.0 |
| Language standard | C++17 |
| Build configuration | Release |
| Reported hardware concurrency | 10 |
| Repetitions | 15 |

### Benchmark results

| Configuration | Average (ms) | Median (ms) | Std. Dev. (ms) | CV (%) | Throughput (tasks/s) |
|---|---:|---:|---:|---:|---:|
| Sequential | 6.30 | 6.36 | 0.24 | 3.88 | 15,879,870 |
| Mutex 1P/1C | 16.94 | 16.94 | 0.16 | 0.95 | 5,901,480 |
| Mutex 2P/2C | 22.09 | 22.10 | 0.28 | 1.29 | 4,526,155 |
| Mutex 4P/4C | 33.28 | 33.55 | 1.24 | 3.72 | 3,004,608 |
| Mutex 8P/4C | 27.51 | 27.77 | 0.70 | 2.54 | 3,634,725 |

### Metrics

The benchmark reports:

- Average execution time.
- Median execution time.
- Minimum and maximum execution time.
- Sample standard deviation.
- Coefficient of variation.
- Throughput in tasks per second.

### Observations

- The sequential queue achieved the highest throughput in its single-threaded workload.
- Increasing producer and consumer counts did not consistently increase throughput.
- The 8P/4C configuration performed better than 4P/4C.
- The 1P/1C mutex configuration had the lowest coefficient of variation.

These are observations from one machine and one workload, not universal performance conclusions.

### Benchmark limitations

The sequential benchmark performs enqueue operations followed by dequeue operations on a single thread.

The mutex benchmarks execute concurrent producer-consumer workloads.

Therefore, these measurements do not provide a strictly equivalent comparison of sequential and concurrent queue operations.

Additionally:

- Concurrent measurements include thread creation and joining.
- Atomic instrumentation adds overhead.
- Operating-system scheduling and background activity can influence measurements.
- The benchmark does not establish lock-free performance.

For further details, see [`docs/benchmark-report.md`](docs/benchmark-report.md).

---

## Engineering Concepts

ConLab explores the following systems programming concepts:

**Memory management**
- Dynamic allocation.
- Pointer ownership.
- Destructors.
- Copy semantics.
- Object lifetime.

**Concurrency**
- Shared mutable state.
- Thread safety.
- Mutual exclusion.
- Condition variables.
- Producer-consumer coordination.

**Atomic operations**
- Atomic variables.
- Read-modify-write operations.
- Compare-and-swap.
- Memory ordering.

**Performance engineering**
- Workload design.
- Throughput.
- Execution-time measurements.
- Statistical variability.
- Benchmark limitations.

---

## Limitations

ConLab is an educational implementation and experimental laboratory.

Current limitations include:

- The concurrent queue uses mutex-based synchronization rather than a lock-free algorithm.
- The benchmark configurations do not all represent equivalent workloads.
- Thread creation and joining are included in concurrent benchmark timings.
- The current benchmark does not isolate all sources of synchronization overhead.
- The test harness is not yet organized as a separate automated unit-testing framework.

These limitations provide opportunities for future engineering improvements.

---

## Development Roadmap

| Phase | Focus | Status |
|---|---|---|
| 1 | Sequential linked-list queue | Complete |
| 2 | Memory management and copy semantics | Complete |
| 3 | Mutex-protected concurrent queue | Complete |
| 4 | Atomic counters and CAS | Complete |
| 5 | Producer-consumer stress testing | Complete |
| 6 | Benchmarking and statistical analysis | Complete |
| 7 | Engineering documentation | In progress |

---

## Future Improvements

Potential future work includes:

- Separate automated unit-testing infrastructure.
- ThreadSanitizer experiments.
- More controlled benchmark workloads.
- Persistent worker threads for benchmarking.
- Improved measurement isolation.
- Additional queue implementations.
- Further investigation of synchronization and memory ordering.

---

## Project Philosophy

ConLab follows an implementation-first engineering approach:

1. Understand the underlying concept.
2. Design the data structure.
3. Implement it from scratch.
4. Test correctness.
5. Investigate edge cases.
6. Measure performance.
7. Document engineering decisions.

The goal is not simply to implement concurrent data structures, but to understand the underlying memory, synchronization, and execution models.

**Correctness and understanding take priority over implementation speed.**