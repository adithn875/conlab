
#include "sequential_queue.hpp"
#include <iostream>
#include <utility>
#include <stdexcept>

// Default constructor
SequentialQueue::SequentialQueue() {
    head = nullptr;
    tail = nullptr;
    count = 0;
}

// Enqueue
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

    count++;
}

// Dequeue
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
    count--;
}

// Front
int SequentialQueue::front() const {
    if (head == nullptr) {
        throw std::out_of_range("Queue is empty");
    }

    return head->data;
}

// Size
int SequentialQueue::size() const {
    return count;
}

// Check if empty
bool SequentialQueue::is_empty() const {
    return head == nullptr;
}

// Display
void SequentialQueue::display() const {
    Node* current = head;

    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }

    std::cout << std::endl;
}

// Destructor
SequentialQueue::~SequentialQueue() {
    Node* current = head;

    while (current != nullptr) {
        Node* temp = current->next;
        delete current;
        current = temp;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

// Exception-safe copy constructor
SequentialQueue::SequentialQueue(const SequentialQueue& other) {
    head = nullptr;
    tail = nullptr;
    count = 0;

    Node* temp = other.head;

    try {
        while (temp != nullptr) {
            Node* new_node = new Node(temp->data);

            if (head == nullptr) {
                head = new_node;
                tail = new_node;
            }
            else {
                tail->next = new_node;
                tail = new_node;
            }

            count++;
            temp = temp->next;
        }
    }
    catch (...) {
        Node* current = head;

        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
        tail = nullptr;
        count = 0;

        throw;
    }
}

// Copy assignment operator
SequentialQueue& SequentialQueue::operator=(
    const SequentialQueue& other
) {
    if (this == &other) {
        return *this;
    }

    SequentialQueue temp(other);

    std::swap(head, temp.head);
    std::swap(tail, temp.tail);
    std::swap(count, temp.count);

    return *this;
}
