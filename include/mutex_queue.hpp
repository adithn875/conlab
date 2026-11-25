
#ifndef MUTEX_QUEUE_HPP
#define MUTEX_QUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <cstddef>

class MutexQueue {
private:
    struct Node {
        int data;
        Node* next;

        explicit Node(int value)
            : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t count;

    bool closed;

    mutable std::mutex mutex;
    std::condition_variable condition;

public:
    MutexQueue();

    // Prevent copying a queue containing a mutex
    MutexQueue(const MutexQueue&) = delete;
    MutexQueue& operator=(const MutexQueue&) = delete;

    bool enqueue(int value);

    // Returns false when the queue is closed and empty
    bool wait_dequeue(int& value);

    void close();

    std::size_t size() const;
    bool is_empty() const;

    ~MutexQueue();
};

#endif
