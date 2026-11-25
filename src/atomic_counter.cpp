
#include "atomic_counter.hpp"

// =====================================
// 1. Normal Counter
// =====================================

NormalCounter::NormalCounter() : count(0) {
}

void NormalCounter::increment() {
    count++;
}

int NormalCounter::get() const {
    return count;
}

// =====================================
// 2. Mutex Counter
// =====================================

MutexCounter::MutexCounter() : count(0) {
}

void MutexCounter::increment() {
    std::lock_guard<std::mutex> lock(mutex);
    count++;
}

int MutexCounter::get() {
    std::lock_guard<std::mutex> lock(mutex);
    return count;
}

// =====================================
// 3. Atomic Counter
// =====================================

AtomicCounter::AtomicCounter() : count(0) {
}

// Sequentially consistent increment
void AtomicCounter::increment() {
    count.fetch_add(1);
}

// Compare-and-Swap increment
void AtomicCounter::increment_cas() {
    int current = count.load();

    while (!count.compare_exchange_weak(current, current + 1)) {
    }
}

// Relaxed ordering increment
void AtomicCounter::increment_relaxed() {
    count.fetch_add(1, std::memory_order_relaxed);
}

int AtomicCounter::get() const {
    return count.load();
}
