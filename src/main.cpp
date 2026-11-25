
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "mutex_queue.hpp"

int main() {
    const int number_of_runs = 10;

    const int producer_count = 4;
    const int consumer_count = 3;
    const int items_per_producer = 1000;

    const int expected_count =
        producer_count * items_per_producer;

    const long long expected_sum =
        static_cast<long long>(expected_count) *
        (expected_count + 1) / 2;

    // Repeat the complete concurrency test 10 times
    for (int run = 1; run <= number_of_runs; run++) {

        MutexQueue queue;

        std::atomic<int> consumed_count{0};
        std::atomic<long long> consumed_sum{0};

        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;

        // Start consumer threads
        for (int i = 0; i < consumer_count; i++) {
            consumers.emplace_back([&]() {
                int value;

                while (queue.wait_dequeue(value)) {
                    consumed_count++;
                    consumed_sum += value;
                }
            });
        }

        // Start producer threads
        for (int i = 0; i < producer_count; i++) {
            producers.emplace_back([&, i]() {

                int start = i * items_per_producer + 1;
                int end = start + items_per_producer;

                for (int value = start; value < end; value++) {
                    queue.enqueue(value);
                }

            });
        }

        // Wait for all producers to finish
        for (auto& producer : producers) {
            producer.join();
        }

        // Close queue after all producers finish
        queue.close();

        // Wait for all consumers to finish
        for (auto& consumer : consumers) {
            consumer.join();
        }

        // Verify results
        bool passed =
            consumed_count.load() == expected_count &&
            consumed_sum.load() == expected_sum &&
            queue.size() == 0 &&
            queue.is_empty();

        if (passed) {
            std::cout << "Run " << run << ": PASSED"
                      << std::endl;
        } else {
            std::cout << "Run " << run << ": FAILED"
                      << std::endl;

            std::cout << "Expected count: " << expected_count
                      << ", Actual count: " << consumed_count.load()
                      << std::endl;

            std::cout << "Expected sum: " << expected_sum
                      << ", Actual sum: " << consumed_sum.load()
                      << std::endl;

            std::cout << "Final queue size: "
                      << queue.size() << std::endl;

            return 1;
        }
    }

    std::cout << "\nAll " << number_of_runs
              << " stress tests passed!" << std::endl;

    return 0;
}
