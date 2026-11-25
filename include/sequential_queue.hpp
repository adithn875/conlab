
#ifndef SEQUENTIAL_QUEUE_HPP
#define SEQUENTIAL_QUEUE_HPP

#include <cstddef>

class SequentialQueue {
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

public:
    SequentialQueue();

    SequentialQueue(const SequentialQueue& other);
    SequentialQueue& operator=(const SequentialQueue& other);

    void enqueue(int value);
    void dequeue();

    int front() const;
    std::size_t size() const;
    bool is_empty() const;

    void display() const;

    ~SequentialQueue();
};

#endif
