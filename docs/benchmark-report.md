# ConLab — Queue Benchmark Report

**Project:** ConLab — Concurrent Data Structures Laboratory  
**Author:** Adith N K  
**Language:** C++17  
**Compiler:** Apple LLVM 21.0.0 (clang-2100.1.1.101)  
**Operating System:** macOS  
**Build Configuration:** Release  
**Reported Hardware Concurrency:** 10  
**Benchmark Repetitions:** 15  
**Tasks per Configuration:** 100,000

---

## 1. Objective

The objective of this experiment is to evaluate the performance of two queue implementations developed in ConLab:

- Sequential linked-list queue.
- Mutex-protected concurrent queue.

The experiment investigates how execution time and throughput change across different producer-consumer configurations.

It also examines measurement variability using statistical metrics.

## 2. Implementations

### 2.1 Sequential Queue

The sequential queue uses a linked-list implementation with:

- Dynamic node allocation.
- Head and tail pointers.
- FIFO ordering.
- No synchronization primitives.

All queue operations execute on a single thread.

### 2.2 Mutex Queue

The concurrent queue uses:

- A linked-list implementation.
- `std::mutex` for mutual exclusion.
- `std::lock_guard` for protected operations.
- `std::unique_lock` for condition-variable waiting.
- `std::condition_variable` for consumer notification.
- A close mechanism for graceful shutdown.

Multiple producers and consumers can operate on the queue concurrently.

## 3. Experimental Configurations

Five configurations were evaluated.

| Configuration | Producers | Consumers |
|---|---:|---:|
| Sequential queue | 1 | 1 |
| Mutex queue | 1 | 1 |
| Mutex queue | 2 | 2 |
| Mutex queue | 4 | 4 |
| Mutex queue | 8 | 4 |

Each configuration processes 100,000 tasks per repetition.

Each benchmark was executed 15 times.

## 4. Measurement Methodology

The benchmark uses `std::chrono::steady_clock` to measure elapsed time.

The following metrics are collected:

**Average execution time**

Arithmetic mean of the 15 execution-time observations.

**Median execution time**

Middle observation after sorting the execution times.

**Minimum and maximum execution time**

Fastest and slowest observed runs.

**Standard deviation**

Sample standard deviation, measuring the spread of execution times around the mean.

**Coefficient of variation**

Standard deviation expressed as a percentage of the average.

**Throughput**

Number of tasks processed per second, calculated using average elapsed time:

Throughput = Tasks / Average Execution Time in Seconds

Correctness checks are also performed to verify that the expected number of tasks is consumed and that the queue is empty after completion.

## 5. Experimental Results

### 5.1 Performance Measurements

Results from the Release build:

| Configuration | Average (ms) | Median (ms) | Min (ms) | Max (ms) | StdDev (ms) | CV (%) | Throughput (tasks/s) |
|---|---:|---:|---:|---:|---:|---:|---:|
| Sequential | 6.30 | 6.36 | 5.93 | 6.78 | 0.24 | 3.88 | 15,879,870 |
| Mutex 1P/1C | 16.94 | 16.94 | 16.67 | 17.21 | 0.16 | 0.95 | 5,901,480 |
| Mutex 2P/2C | 22.09 | 22.10 | 21.72 | 22.69 | 0.28 | 1.29 | 4,526,155 |
| Mutex 4P/4C | 33.28 | 33.55 | 31.65 | 36.33 | 1.24 | 3.72 | 3,004,608 |
| Mutex 8P/4C | 27.51 | 27.77 | 26.10 | 28.16 | 0.70 | 2.54 | 3,634,725 |

All five configurations passed their correctness checks.

### 5.2 Throughput Observations

The sequential queue achieved the highest measured throughput in its single-threaded workload.

Among the mutex queue configurations:

- 1 producer / 1 consumer achieved approximately 5.90 million tasks per second.
- 2 producers / 2 consumers achieved approximately 4.53 million tasks per second.
- 4 producers / 4 consumers achieved approximately 3.00 million tasks per second.
- 8 producers / 4 consumers achieved approximately 3.63 million tasks per second.

Increasing the number of producers and consumers did not consistently increase throughput.

### 5.3 Statistical Observations

The 1P/1C mutex configuration had the lowest coefficient of variation at 0.95%.

The 4P/4C configuration had the highest coefficient of variation at 3.72%.

This indicates that the 4P/4C measurements showed greater relative variability than the other configurations.

The measurements were reasonably consistent across the 15 repetitions, although variability remains dependent on system scheduling and runtime conditions.

## 6. Correctness Validation

Each benchmark includes correctness checks.

The validation covers:

- Expected number of consumed tasks.
- Correct sum of task identifiers.
- Successful queue shutdown.
- Empty queue after completion.

All five benchmark configurations passed these checks.

Earlier Phase 5 testing also covered:

- Missing task detection.
- Duplicate task detection.
- Invalid task detection.
- Failed enqueue detection.
- Multiple producer-consumer combinations.
- Queue closure with pending tasks.
- Rejection of enqueue operations after closure.
- Repeated stress testing.

## 7. Interpretation

The experiment demonstrates that synchronization introduces additional work compared with the sequential implementation.

The mutex queue must coordinate access between producers and consumers, while the sequential queue operates without synchronization.

The results also demonstrate that adding threads does not automatically improve throughput.

The 8P/4C configuration performed better than 4P/4C, but neither exceeded the 1P/1C mutex configuration.

These observations are consistent with the possibility of mutex contention, scheduling overhead, and coordination costs affecting performance.

However, the experiment does not isolate the individual contribution of each factor.

## 8. Limitations

The following limitations must be considered when interpreting these measurements.

### 8.1 Different Workloads

The sequential benchmark performs enqueue operations followed by dequeue operations on one thread.

The mutex benchmarks use concurrent producers and consumers.

Therefore, the throughput values are not a controlled, apples-to-apples comparison of synchronization overhead.

### 8.2 Thread Lifecycle Overhead

The concurrent benchmark includes thread creation and joining within the measured interval.

Consequently, execution time includes thread lifecycle costs in addition to queue operations.

### 8.3 Measurement Instrumentation

The concurrent implementations use atomic counters to record consumed tasks and their sum.

This instrumentation adds work to the measured execution.

### 8.4 Environment Variability

Operating-system scheduling, background processes, CPU frequency changes, and other system activity may influence execution time.

The results should be treated as measurements from one machine and one experimental setup.

### 8.5 Scope

This experiment compares a sequential queue and a mutex-protected queue.

It does not establish the performance of lock-free queues or other concurrent data structures.

## 9. Conclusion

The ConLab benchmark demonstrates the functional correctness of both queue implementations under the tested workloads.

The sequential queue achieved the highest throughput in its single-threaded workload.

The mutex queue successfully supported multiple producers and consumers, although increasing concurrency did not consistently improve throughput.

The statistical measurements provided additional insight into execution-time variability.

This experiment establishes a useful baseline for future work involving concurrent data structures, synchronization strategies, and performance measurement.

---

**Project:** ConLab  
**Report status:** Initial Release Benchmark Report  
**Benchmark repetitions:** 15 per configuration  
**Correctness status:** All five configurations passed