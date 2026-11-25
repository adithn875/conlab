
#include "mutex_queue.hpp"

// Constructor
MutexQueue::MutexQueue()
    : head(nullptr),
      tail(nullptr),
      count(0),
      closed(false) {
}

// Enqueue
bool MutexQueue::enqueue(int value) {
    Node* new_node = new Node(value);

    {
        std::lock_guard<std::mutex> lock(mutex);

        if (closed) {
            delete new_node;
            return false;
        }

        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        count++;
    }

    condition.notify_one();
    return true;
}

// Wait and dequeue
bool MutexQueue::wait_dequeue(int& value) {
    std::unique_lock<std::mutex> lock(mutex);

    condition.wait(lock, [this] {
        return head != nullptr || closed;
    });

    if (head == nullptr) {
        return false;
    }

    Node* temp = head;
    value = temp->data;

    head = head->next;

    if (head == nullptr) {
        tail = nullptr;
    }

    delete temp;
    count--;

    return true;
}

// Close queue
void MutexQueue::close() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        closed = true;
    }

    condition.notify_all();
}

// Size
std::size_t MutexQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex);
    return count;
}

// Empty check
bool MutexQueue::is_empty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return head == nullptr;
}

// Destructor
MutexQueue::~MutexQueue() {
    Node* current = head;

    while (current != nullptr) {
        Node* temp = current->next;
        delete current;
        current = temp;
    }
}
