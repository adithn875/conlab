
#ifndef SEQUENTIAL_QUEUE_HPP
#define SEQUENTIAL_QUEUE_HPP

struct Node {
    int data;
    Node* next;

    Node(int value) {
        data = value;
        next = nullptr;
    }
};

class SequentialQueue {
private:
    Node* head;
    Node* tail;
    int count;

public:
    // Constructor
    SequentialQueue();

    // Copy constructor
    SequentialQueue(const SequentialQueue& other);

    // Copy assignment operator
    SequentialQueue& operator=(const SequentialQueue& other);

    // Queue operations
    void enqueue(int value);
    void dequeue();

    int front() const;
    int size() const;
    bool is_empty() const;

    void display() const;

    // Destructor
    ~SequentialQueue();
};

#endif
