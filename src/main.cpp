
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

#include "mutex_queue.hpp"

void test_close_empty_queue() {
    std::cout << "\n===== TEST 1: CLOSE EMPTY QUEUE =====\n";

    MutexQueue queue;

    constexpr int NUM_CONSUMERS = 4;
    std::atomic<int> exited{0};

    std::vector<std::thread> consumers;

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumers.emplace_back([&]() {
            int value;

            if (!queue.wait_dequeue(value)) {
                exited++;
            }
        });
    }

    // Allow consumers to start waiting.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    queue.close();

    for (auto& consumer : consumers) {
        consumer.join();
    }

    std::cout << "Consumers expected to exit: "
              << NUM_CONSUMERS << std::endl;

    std::cout << "Consumers exited: "
              << exited.load() << std::endl;

    std::cout << "Result: "
              << (exited == NUM_CONSUMERS ? "PASSED" : "FAILED")
              << std::endl;
}

void test_close_with_pending_tasks() {
    std::cout << "\n===== TEST 2: CLOSE WITH PENDING TASKS =====\n";

    MutexQueue queue;

    constexpr int NUM_TASKS = 100;

    for (int i = 1; i <= NUM_TASKS; i++) {
        queue.enqueue(i);
    }

    // Close before starting consumers.
    queue.close();

    std::atomic<int> consumed{0};
    std::atomic<long long> sum{0};

    std::vector<std::thread> consumers;

    for (int i = 0; i < 4; i++) {
        consumers.emplace_back([&]() {
            int value;

            while (queue.wait_dequeue(value)) {
                consumed++;
                sum += value;
            }
        });
    }

    for (auto& consumer : consumers) {
        consumer.join();
    }

    const long long expected_sum =
        static_cast<long long>(NUM_TASKS) * (NUM_TASKS + 1) / 2;

    bool passed =
        consumed == NUM_TASKS &&
        sum == expected_sum &&
        queue.size() == 0;

    std::cout << "Expected tasks: " << NUM_TASKS << std::endl;
    std::cout << "Consumed tasks: " << consumed.load() << std::endl;
    std::cout << "Expected sum: " << expected_sum << std::endl;
    std::cout << "Actual sum: " << sum.load() << std::endl;
    std::cout << "Final queue size: " << queue.size() << std::endl;

    std::cout << "Result: "
              << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_enqueue_after_close() {
    std::cout << "\n===== TEST 3: ENQUEUE AFTER CLOSE =====\n";

    MutexQueue queue;

    bool first = queue.enqueue(10);

    queue.close();

    bool second = queue.enqueue(20);

    int value = -1;
    bool dequeued = queue.wait_dequeue(value);

    bool third = queue.wait_dequeue(value);

    bool passed =
        first &&
        !second &&
        dequeued &&
        value == 10 &&
        !third &&
        queue.size() == 0;

    std::cout << "Enqueue before close: "
              << first << std::endl;

    std::cout << "Enqueue after close: "
              << second << std::endl;

    std::cout << "First dequeue successful: "
              << dequeued << std::endl;

    std::cout << "Dequeued value: "
              << value << std::endl;

    std::cout << "Second dequeue successful: "
              << third << std::endl;

    std::cout << "Final queue size: "
              << queue.size() << std::endl;

    std::cout << "Result: "
              << (passed ? "PASSED" : "FAILED") << std::endl;
}

int main() {
    test_close_empty_queue();

    test_close_with_pending_tasks();

    test_enqueue_after_close();

    return 0;
}
