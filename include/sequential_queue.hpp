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

public:
    SequentialQueue();

    void enqueue(int value);
    bool is_empty();
    void display();
    void dequeue();
};

#endif