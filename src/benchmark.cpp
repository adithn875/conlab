
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "sequential_queue.hpp"
#include "mutex_queue.hpp"

using Clock = std::chrono::steady_clock;

constexpr int TASKS = 100000;
constexpr int REPETITIONS = 15;

struct BenchmarkResult {
    std::string name;
    double average_ms;
    double median_ms;
    double min_ms;
    double max_ms;
    double std_deviation;
    double coefficient_variation;
    double throughput;
    bool passed;
};

// =====================================
// Statistical calculations
// =====================================

double calculate_median(std::vector<double> values) {
    std::sort(values.begin(), values.end());

    std::size_t middle = values.size() / 2;

    if (values.size() % 2 == 0) {
        return (values[middle - 1] + values[middle]) / 2.0;
    }

    return values[middle];
}

double calculate_std_deviation(
    const std::vector<double>& values,
    double average
) {
    if (values.size() < 2) {
        return 0.0;
    }

    double squared_sum = 0.0;

    for (double value : values) {
        double difference = value - average;
        squared_sum += difference * difference;
    }

    // Sample standard deviation
    return std::sqrt(squared_sum / (values.size() - 1));
}

BenchmarkResult summarize(
    const std::string& name,
    const std::vector<double>& times,
    int tasks,
    bool passed
) {
    double total = std::accumulate(
        times.begin(), times.end(), 0.0
    );

    double average = total / times.size();

    double median = calculate_median(times);

    double minimum = *std::min_element(
        times.begin(), times.end()
    );

    double maximum = *std::max_element(
        times.begin(), times.end()
    );

    double std_deviation =
        calculate_std_deviation(times, average);

    double cv = average > 0
        ? (std_deviation / average) * 100.0
        : 0.0;

    double throughput = tasks / (average / 1000.0);

    return {
        name,
        average,
        median,
        minimum,
        maximum,
        std_deviation,
        cv,
        throughput,
        passed
    };
}

// =====================================
// Print benchmark result
// =====================================

void print_result(const BenchmarkResult& result) {
    std::cout
        << std::left << std::setw(23) << result.name
        << std::right
        << std::setw(10) << std::fixed << std::setprecision(2)
        << result.average_ms
        << std::setw(10) << result.median_ms
        << std::setw(10) << result.min_ms
        << std::setw(10) << result.max_ms
        << std::setw(10) << result.std_deviation
        << std::setw(9) << result.coefficient_variation
        << std::setw(15) << std::setprecision(0)
        << result.throughput
        << std::setw(10)
        << (result.passed ? "PASSED" : "FAILED")
        << '\n';
}

// =====================================
// 1. Sequential Queue
// =====================================

BenchmarkResult benchmark_sequential() {
    std::vector<double> times;
    bool all_passed = true;

    for (int run = 0; run < REPETITIONS; run++) {
        SequentialQueue queue;

        auto start = Clock::now();

        for (int i = 0; i < TASKS; i++) {
            queue.enqueue(i);
        }

        bool passed = true;

        for (int i = 0; i < TASKS; i++) {
            if (queue.is_empty() || queue.front() != i) {
                passed = false;
                break;
            }

            queue.dequeue();
        }

        auto end = Clock::now();

        double milliseconds =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        times.push_back(milliseconds);

        passed = passed && queue.is_empty();
        all_passed = all_passed && passed;
    }

    return summarize(
        "Sequential queue",
        times,
        TASKS,
        all_passed
    );
}

// =====================================
// 2. Mutex Queue: 1 Producer / 1 Consumer
// =====================================

BenchmarkResult benchmark_mutex_single() {
    std::vector<double> times;
    bool all_passed = true;

    for (int run = 0; run < REPETITIONS; run++) {
        MutexQueue queue;

        std::atomic<int> consumed{0};
        std::atomic<long long> sum{0};

        auto start = Clock::now();

        std::thread consumer([&]() {
            int value;

            while (queue.wait_dequeue(value)) {
                consumed++;
                sum += value;
            }
        });

        std::thread producer([&]() {
            for (int i = 0; i < TASKS; i++) {
                queue.enqueue(i);
            }

            queue.close();
        });

        producer.join();
        consumer.join();

        auto end = Clock::now();

        double milliseconds =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        times.push_back(milliseconds);

        long long expected_sum =
            static_cast<long long>(TASKS) * (TASKS - 1) / 2;

        bool passed =
            consumed == TASKS &&
            sum == expected_sum &&
            queue.size() == 0;

        all_passed = all_passed && passed;
    }

    return summarize(
        "Mutex (1P / 1C)",
        times,
        TASKS,
        all_passed
    );
}

// =====================================
// 3. Mutex Queue: Multiple Producers / Consumers
// =====================================

BenchmarkResult benchmark_mutex_multi(
    int num_producers,
    int num_consumers
) {
    std::vector<double> times;
    bool all_passed = true;

    const int tasks_per_producer = TASKS / num_producers;
    const int total_tasks = tasks_per_producer * num_producers;

    for (int run = 0; run < REPETITIONS; run++) {
        MutexQueue queue;

        std::atomic<int> consumed{0};
        std::atomic<long long> sum{0};

        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;

        auto start = Clock::now();

        for (int i = 0; i < num_consumers; i++) {
            consumers.emplace_back([&]() {
                int value;

                while (queue.wait_dequeue(value)) {
                    consumed++;
                    sum += value;
                }
            });
        }

        for (int i = 0; i < num_producers; i++) {
            producers.emplace_back([&, i]() {
                int base = i * tasks_per_producer;

                for (int j = 0; j < tasks_per_producer; j++) {
                    queue.enqueue(base + j);
                }
            });
        }

        for (auto& producer : producers) {
            producer.join();
        }

        queue.close();

        for (auto& consumer : consumers) {
            consumer.join();
        }

        auto end = Clock::now();

        double milliseconds =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        times.push_back(milliseconds);

        long long expected_sum = 0;

        for (int i = 0; i < num_producers; i++) {
            long long base =
                static_cast<long long>(i) * tasks_per_producer;

            expected_sum +=
                static_cast<long long>(tasks_per_producer) *
                (2 * base + tasks_per_producer - 1) / 2;
        }

        bool passed =
            consumed == total_tasks &&
            sum == expected_sum &&
            queue.size() == 0;

        all_passed = all_passed && passed;
    }

    std::string name =
        "Mutex (" + std::to_string(num_producers) +
        "P / " + std::to_string(num_consumers) + "C)";

    return summarize(
        name,
        times,
        total_tasks,
        all_passed
    );
}

// =====================================
// 4. System Information
// =====================================

void print_system_info() {
    std::cout << "\n========== SYSTEM INFORMATION ==========\n";

#ifdef __VERSION__
    std::cout << "Compiler: " << __VERSION__ << '\n';
#endif

#ifdef __APPLE__
    std::cout << "Operating system: macOS\n";
#elif defined(__linux__)
    std::cout << "Operating system: Linux\n";
#elif defined(_WIN32)
    std::cout << "Operating system: Windows\n";
#else
    std::cout << "Operating system: Unknown\n";
#endif

    std::cout << "C++ standard: C++17\n";

    std::cout << "Hardware concurrency reported: "
              << std::thread::hardware_concurrency()
              << '\n';

    std::cout << "Tasks per benchmark: " << TASKS << '\n';
    std::cout << "Repetitions: " << REPETITIONS << '\n';
}

// =====================================
// Main
// =====================================

int main() {
    print_system_info();

    std::vector<BenchmarkResult> results;

    std::cout << "\nRunning benchmarks...\n";

    results.push_back(benchmark_sequential());
    results.push_back(benchmark_mutex_single());
    results.push_back(benchmark_mutex_multi(2, 2));
    results.push_back(benchmark_mutex_multi(4, 4));
    results.push_back(benchmark_mutex_multi(8, 4));

    std::cout << "\n================ RESULTS ================\n";

    std::cout
        << std::left << std::setw(23) << "Benchmark"
        << std::right
        << std::setw(10) << "Average"
        << std::setw(10) << "Median"
        << std::setw(10) << "Min"
        << std::setw(10) << "Max"
        << std::setw(10) << "StdDev"
        << std::setw(9) << "CV%"
        << std::setw(15) << "Tasks/s"
        << std::setw(10) << "Status"
        << '\n';

    std::cout << std::string(107, '-') << '\n';

    bool all_passed = true;

    for (const auto& result : results) {
        print_result(result);
        all_passed = all_passed && result.passed;
    }

    std::cout << "\nOverall result: "
              << (all_passed ? "ALL PASSED" : "FAILURE DETECTED")
              << '\n';

    return all_passed ? 0 : 1;
}
