#include "sequential_queue.hpp"
#include <iostream>

SequentialQueue::SequentialQueue() {
    head = nullptr;
    tail = nullptr;
}

void SequentialQueue::enqueue(int value) {
    Node* new_node = new Node(value);

    if (head == nullptr) {
        head = new_node;
        tail = new_node;
    }
    else {
        tail->next = new_node;
        tail = new_node;
    }
}

bool SequentialQueue::is_empty() {
    return head == nullptr;
}

void SequentialQueue::display() {
    Node* current = head;

    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }

    std::cout << std::endl;
}

void SequentialQueue::dequeue() {

    if (head == nullptr) {
        return;
    }

    Node* temp = head;

    head = head->next;

    if (head == nullptr) {
        tail = nullptr;
    }

    delete temp;
}