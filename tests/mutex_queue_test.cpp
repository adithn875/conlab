
#include "mutex_queue.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// Test 1: Basic enqueue and dequeue
void test_enqueue_dequeue() {
    MutexQueue queue;

    assert(queue.enqueue(10));
    assert(queue.enqueue(20));

    int value = 0;

    assert(queue.wait_dequeue(value));
    assert(value == 10);

    assert(queue.wait_dequeue(value));
    assert(value == 20);

    queue.close();

    assert(!queue.wait_dequeue(value));

    std::cout << "Basic enqueue/dequeue: PASSED\n";
}

// Test 2: Close an empty queue with waiting consumers
void test_close_empty_queue() {
    MutexQueue queue;

    constexpr int CONSUMERS = 4;
    std::atomic<int> exited{0};

    std::vector<std::thread> consumers;

    for (int i = 0; i < CONSUMERS; ++i) {
        consumers.emplace_back([&]() {
            int value;

            while (queue.wait_dequeue(value)) {
            }

            exited.fetch_add(1);
        });
    }

    queue.close();

    for (auto& consumer : consumers) {
        consumer.join();
    }

    assert(exited.load() == CONSUMERS);

    std::cout << "Close empty queue: PASSED\n";
    std::cout << "Consumers exited: " << exited.load() << '\n';
}

// Test 3: Close a queue containing pending tasks
void test_close_with_pending_tasks() {
    MutexQueue queue;

    constexpr int TASKS = 100;
    constexpr int EXPECTED_SUM = TASKS * (TASKS + 1) / 2;

    for (int i = 1; i <= TASKS; ++i) {
        assert(queue.enqueue(i));
    }

    queue.close();

    int value = 0;
    int consumed = 0;
    int sum = 0;

    while (queue.wait_dequeue(value)) {
        consumed++;
        sum += value;
    }

    assert(consumed == TASKS);
    assert(sum == EXPECTED_SUM);
    assert(queue.size() == 0);

    std::cout << "Close with pending tasks: PASSED\n";
    std::cout << "Tasks consumed: " << consumed << '\n';
    std::cout << "Expected sum: " << EXPECTED_SUM << '\n';
    std::cout << "Actual sum: " << sum << '\n';
}

// Test 4: Reject enqueue after close
void test_enqueue_after_close() {
    MutexQueue queue;

    assert(queue.enqueue(10));

    queue.close();

    assert(!queue.enqueue(20));

    int value = 0;

    assert(queue.wait_dequeue(value));
    assert(value == 10);

    assert(!queue.wait_dequeue(value));
    assert(queue.size() == 0);

    std::cout << "Enqueue after close: PASSED\n";
}

// Test 5: Multiple producers and consumers
void test_multiple_producers_and_consumers() {
    MutexQueue queue;

    constexpr int PRODUCERS = 4;
    constexpr int CONSUMERS = 4;
    constexpr int TASKS_PER_PRODUCER = 10000;
    constexpr int TOTAL_TASKS = PRODUCERS * TASKS_PER_PRODUCER;

    std::atomic<int> consumed{0};
    std::atomic<long long> sum{0};

    std::vector<std::thread> consumers;
    std::vector<std::thread> producers;

    for (int i = 0; i < CONSUMERS; ++i) {
        consumers.emplace_back([&]() {
            int value;

            while (queue.wait_dequeue(value)) {
                consumed.fetch_add(1);
                sum.fetch_add(value);
            }
        });
    }

    for (int i = 0; i < PRODUCERS; ++i) {
        producers.emplace_back([&, i]() {
            int start = i * TASKS_PER_PRODUCER + 1;
            int end = start + TASKS_PER_PRODUCER;

            for (int value = start; value < end; ++value) {
                bool accepted = queue.enqueue(value);
                assert(accepted);
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

    constexpr long long EXPECTED_SUM =
        static_cast<long long>(TOTAL_TASKS) *
        (TOTAL_TASKS + 1) / 2;

    assert(consumed.load() == TOTAL_TASKS);
    assert(sum.load() == EXPECTED_SUM);
    assert(queue.size() == 0);

    std::cout << "Multiple producers/consumers: PASSED\n";
    std::cout << "Tasks consumed: " << consumed.load() << '\n';
    std::cout << "Expected sum: " << EXPECTED_SUM << '\n';
    std::cout << "Actual sum: " << sum.load() << '\n';
}

// Test 6: Enqueue racing with close
void test_enqueue_close_race() {
    MutexQueue queue;

    constexpr int PRODUCERS = 4;
    constexpr int CONSUMERS = 2;
    constexpr int TASKS_PER_PRODUCER = 10000;

    std::atomic<int> accepted{0};
    std::atomic<int> consumed{0};
    std::atomic<int> producers_started{0};

    std::vector<std::thread> consumers;
    std::vector<std::thread> producers;

    // Start consumers
    for (int i = 0; i < CONSUMERS; ++i) {
        consumers.emplace_back([&]() {
            int value;

            while (queue.wait_dequeue(value)) {
                consumed.fetch_add(1);
            }
        });
    }

    // Start producers
    for (int i = 0; i < PRODUCERS; ++i) {
        producers.emplace_back([&, i]() {
            producers_started.fetch_add(1);

            for (int j = 0; j < TASKS_PER_PRODUCER; ++j) {
                int value = i * TASKS_PER_PRODUCER + j + 1;

                if (queue.enqueue(value)) {
                    accepted.fetch_add(1);
                }
            }
        });
    }

    // Wait until every producer thread has started
    while (producers_started.load() != PRODUCERS) {
        std::this_thread::yield();
    }

    // Give producers an opportunity to enqueue before closing
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    queue.close();

    // Wait for producers
    for (auto& producer : producers) {
        producer.join();
    }

    // Consumers drain remaining accepted tasks and exit
    for (auto& consumer : consumers) {
        consumer.join();
    }

    assert(consumed.load() == accepted.load());
    assert(queue.size() == 0);

    std::cout << "Enqueue/close race: PASSED\n";
    std::cout << "Accepted tasks: " << accepted.load() << '\n';
    std::cout << "Consumed tasks: " << consumed.load() << '\n';
    std::cout << "Final queue size: " << queue.size() << '\n';
}

int main() {
    std::cout << "\n========== MUTEX QUEUE TESTS ==========\n\n";

    test_enqueue_dequeue();
    test_close_empty_queue();
    test_close_with_pending_tasks();
    test_enqueue_after_close();
    test_multiple_producers_and_consumers();
    test_enqueue_close_race();

    std::cout << "\nAll MutexQueue tests passed!\n";

    return 0;
}
