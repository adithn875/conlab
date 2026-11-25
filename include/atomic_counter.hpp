
#ifndef ATOMIC_COUNTER_HPP
#define ATOMIC_COUNTER_HPP

#include <atomic>
#include <mutex>

// 1. Normal counter
class NormalCounter {
private:
    int count;

public:
    NormalCounter();

    void increment();
    int get() const;
};

// 2. Mutex-protected counter
class MutexCounter {
private:
    int count;
    std::mutex mutex;

public:
    MutexCounter();

    void increment();
    int get();
};

// 3. Atomic counter
class AtomicCounter {
private:
    std::atomic<int> count;

public:
    AtomicCounter();

    void increment();
    int get() const;
    void increment_cas();
    void increment_relaxed();
};

#endif
