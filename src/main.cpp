
#include <iostream>
#include <thread>
#include <vector>

#include "atomic_counter.hpp"

int main() {
    constexpr int NUM_THREADS = 4;
    constexpr int INCREMENTS_PER_THREAD = 100000;

    AtomicCounter counter;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < INCREMENTS_PER_THREAD; j++) {
                counter.increment_relaxed();
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Relaxed counter: "
              << counter.get() << std::endl;

    std::cout << "Expected: "
              << NUM_THREADS * INCREMENTS_PER_THREAD << std::endl;

    return 0;
}
