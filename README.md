# ConLab — Concurrent Data Structures Laboratory in C++

**ConLab** is an educational systems programming project focused on understanding data structures, memory management, concurrency and synchronization mechanisms in modern C++.

The project is being developed incrementally, starting with fundamental sequential data structures and progressing towards concurrent implementations using mutexes, condition variables and atomic operations.

Rather than treating concurrency as a black box, ConLab explores how these mechanisms work internally through implementation, experimentation, testing and documentation.

> **Project status:** In development  
> **Language:** C++17  
> **Build system:** CMake

---

## Table of Contents

- [Motivation](#motivation)
- [Project Objectives](#project-objectives)
- [Technology Stack](#technology-stack)
- [Repository Structure](#repository-structure)
- [Current Implementation](#current-implementation)
- [Sequential Queue Architecture](#sequential-queue-architecture)
- [Memory Management](#memory-management)
- [Building and Running](#building-and-running)
- [Testing](#testing)
- [Development Roadmap](#development-roadmap)
- [Learning Approach](#learning-approach)

---

## Motivation

Understanding concurrent programming requires more than knowing how to use threads or mutexes. It requires understanding pointers, memory ownership, object lifetimes, synchronization and the problems that arise when multiple execution contexts access shared data.

ConLab was started to build these foundations from scratch.

The project begins with a linked-list-based FIFO queue to establish a strong understanding of pointers and dynamic memory allocation. It will gradually evolve into a collection of sequential and concurrent data structures.

The emphasis is on understanding the engineering decisions behind each implementation rather than simply producing working code.

---

## Project Objectives

The main objectives are:

- Understand pointers, references and dynamic memory allocation in C++.
- Implement fundamental data structures from scratch.
- Develop a practical understanding of RAII and object lifetime.
- Explore constructors, destructors and copy semantics.
- Understand thread safety and shared mutable state.
- Implement synchronization using mutexes and condition variables.
- Explore atomic operations and compare-and-swap (CAS).
- Test concurrent behaviour under multiple execution threads.
- Benchmark sequential and concurrent implementations.

---

## Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Primary programming language |
| CMake | Build configuration |
| AppleClang | Compiler |
| CLion | Development environment |
| LLDB | Debugging |

Additional testing and analysis tools will be introduced as the project progresses.

---

## Repository Structure

```text
conlab/
│
├── include/
│   └── sequential_queue.hpp
│
├── src/
│   ├── main.cpp
│   └── sequential_queue.cpp
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

### Directory responsibilities

**include/**

Contains header files that declare data structures, class interfaces and public operations.

**src/**

Contains the implementation of the declared classes and the program entry point.

**CMakeLists.txt**

Defines the project configuration, compiler standard and source files required to build the executable.

---

## Current Implementation

### 1. Sequential Queue

The first data structure implemented in ConLab is a linked-list-based FIFO queue.

FIFO stands for **First In, First Out**: the first element inserted is the first element removed.

The implementation uses dynamically allocated nodes connected through pointers.

Each node contains:

```cpp
struct Node {
    int data;
    Node* next;
};
```

The queue maintains two pointers:

- `head`: Points to the first element.
- `tail`: Points to the last element.

### 2. Implemented Operations

| Operation | Description | Status |
|---|---|---|
| Constructor | Initializes an empty queue | Implemented |
| `enqueue(value)` | Inserts an element at the back | Implemented |
| `dequeue()` | Removes the front element | Implemented |
| `is_empty()` | Checks whether the queue is empty | Implemented |
| `display()` | Traverses and displays queue elements | Implemented |

### 3. Queue Invariant

The implementation maintains the following conditions:

**Empty queue:**

```cpp
head == nullptr;
tail == nullptr;
```

**Non-empty queue:**

- Both `head` and `tail` point to valid nodes.
- `head` points to the first node.
- `tail` points to the last node.
- `tail->next == nullptr`.

These invariants are important for maintaining the correctness of queue operations.

---

## Sequential Queue Architecture

Consider inserting three elements:

```cpp
queue.enqueue(10);
queue.enqueue(20);
queue.enqueue(30);
```

The resulting linked list is:

```text
HEAD
 |
 v
[10] -> [20] -> [30] -> nullptr
                    ^
                    |
                   TAIL
```

### Enqueue

The `enqueue()` operation inserts a new node at the end of the linked list.

Two cases are handled:

1. If the queue is empty, both `head` and `tail` point to the newly created node.
2. If the queue is non-empty, the previous tail's `next` pointer is updated, and `tail` moves to the new node.

### Dequeue

The `dequeue()` operation removes the first node.

The implementation:

1. Checks whether the queue is empty.
2. Stores the address of the current head.
3. Moves `head` to the next node.
4. Updates `tail` if the queue becomes empty.
5. Deletes the old head node.

This ensures that removed nodes are released rather than left allocated.

### Complexity

| Operation | Time Complexity |
|---|---|
| `enqueue()` | O(1) |
| `dequeue()` | O(1) |
| `is_empty()` | O(1) |
| `display()` | O(n) |

The queue maintains direct access to both its front and back, allowing insertion and removal without traversing the linked list.

---

## Memory Management

An important part of ConLab is understanding manual memory management.

Nodes are dynamically allocated using `new` and released using `delete`.

For example:

```cpp
Node* new_node = new Node(value);
```

The queue must ensure that allocated nodes are eventually released.

Memory ownership, destructors and RAII are part of the upcoming implementation work.

---

## Building and Running

### Prerequisites

- C++17-compatible compiler
- CMake 3.20 or later
- Git

### Clone the repository

```bash
git clone https://github.com/adithn875/conlab.git
cd conlab
```

### Configure the project

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Run

On macOS or Linux:

```bash
./build/conlab
```

---

## Testing

The current implementation has been manually tested using the program entry point.

Verified scenarios include:

- Creating an empty queue.
- Checking the empty state.
- Inserting multiple elements.
- Removing the front element.
- Removing the only element from a queue.
- Checking the empty state after removal.

Automated unit tests, edge-case coverage and concurrency stress tests are planned.

---

## Development Roadmap

ConLab is being developed incrementally.

### Phase 1 — Sequential Data Structures

- [x] Node structure and pointer fundamentals
- [x] Dynamic memory allocation
- [x] Linked-list traversal
- [x] Sequential queue class
- [x] Constructor
- [x] `enqueue()`
- [x] `dequeue()`
- [x] `is_empty()`
- [x] Queue display
- [ ] Destructor and RAII
- [ ] Copy constructor
- [ ] Copy assignment operator
- [ ] Automated tests
- [ ] Memory debugging

### Phase 2 — Thread Safety and Synchronization

- [ ] Understand threads and shared mutable state
- [ ] Implement a mutex-protected queue
- [ ] Explore `std::mutex`
- [ ] Understand `std::lock_guard`
- [ ] Explore `std::unique_lock`
- [ ] Implement condition-variable-based waiting
- [ ] Build producer-consumer experiments

### Phase 3 — Atomic Operations

- [ ] Understand atomic variables
- [ ] Implement an atomic counter
- [ ] Explore `fetch_add()`
- [ ] Understand compare-and-swap
- [ ] Experiment with `compare_exchange_weak()`
- [ ] Experiment with `compare_exchange_strong()`

### Phase 4 — Concurrency Testing

- [ ] Multi-threaded queue testing
- [ ] Producer-consumer stress testing
- [ ] Investigate race conditions
- [ ] Debug synchronization problems
- [ ] Explore correctness under concurrent workloads

### Phase 5 — Benchmarking and Documentation

- [ ] Benchmark sequential and mutex-protected queues
- [ ] Analyse performance under different workloads
- [ ] Document implementation decisions
- [ ] Record experimental results

---

## Learning Approach

ConLab follows an implementation-first learning approach.

Each component is built incrementally:

1. Understand the underlying concept.
2. Design the data structure.
3. Implement it from scratch.
4. Test its behaviour.
5. Investigate errors and edge cases.
6. Document the engineering decisions.

The objective is not just to implement data structures, but to develop a deeper understanding of how C++ manages memory, objects and concurrent execution.

---

## Project Status

ConLab is an ongoing learning and engineering project. The current focus is completing the sequential queue implementation, strengthening memory management and establishing correctness before introducing concurrency.

**Correctness and understanding take priority over implementation speed.**
