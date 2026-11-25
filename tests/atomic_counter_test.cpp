
#include "atomic_counter.hpp"

#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

constexpr int THREADS = 8;
constexpr int INCREMENTS = 100000;
constexpr int EXPECTED = THREADS * INCREMENTS;

void test_normal_counter() {
    NormalCounter counter;

    for (int i = 0; i < 1000; ++i) {
        counter.increment();
    }

    assert(counter.get() == 1000);

    std::cout << "NormalCounter: PASSED\n";
}

void test_mutex_counter() {
    MutexCounter counter;
    std::vector<std::thread> workers;

    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < INCREMENTS; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    assert(counter.get() == EXPECTED);

    std::cout << "MutexCounter: PASSED\n";
    std::cout << "Final count: " << counter.get() << '\n';
}

void test_atomic_counter() {
    AtomicCounter counter;
    std::vector<std::thread> workers;

    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < INCREMENTS; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    assert(counter.get() == EXPECTED);

    std::cout << "AtomicCounter: PASSED\n";
    std::cout << "Final count: " << counter.get() << '\n';
}

void test_cas_counter() {
    AtomicCounter counter;
    std::vector<std::thread> workers;

    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < INCREMENTS; ++j) {
                counter.increment_cas();
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    assert(counter.get() == EXPECTED);

    std::cout << "CAS Counter: PASSED\n";
    std::cout << "Final count: " << counter.get() << '\n';
}

void test_relaxed_counter() {
    AtomicCounter counter;
    std::vector<std::thread> workers;

    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < INCREMENTS; ++j) {
                counter.increment_relaxed();
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    assert(counter.get() == EXPECTED);

    std::cout << "Relaxed Counter: PASSED\n";
    std::cout << "Final count: " << counter.get() << '\n';
}

int main() {
    std::cout << "\n========== ATOMIC COUNTER TESTS ==========\n\n";

    test_normal_counter();
    test_mutex_counter();
    test_atomic_counter();
    test_cas_counter();
    test_relaxed_counter();

    std::cout << "\nAll AtomicCounter tests passed!\n";

    return 0;
}
