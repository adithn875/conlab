
#include "sequential_queue.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

void test_empty_queue() {
    SequentialQueue queue;

    assert(queue.is_empty());
    assert(queue.size() == 0);

    bool exception_thrown = false;

    try {
        queue.front();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

void test_enqueue_and_dequeue() {
    SequentialQueue queue;

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    assert(queue.size() == 3);
    assert(queue.front() == 10);

    queue.dequeue();

    assert(queue.front() == 20);
    assert(queue.size() == 2);

    queue.dequeue();
    queue.dequeue();

    assert(queue.is_empty());
    assert(queue.size() == 0);
}

void test_copy_constructor() {
    SequentialQueue original;

    original.enqueue(10);
    original.enqueue(20);

    SequentialQueue copied(original);

    assert(copied.size() == 2);
    assert(copied.front() == 10);

    copied.enqueue(30);

    assert(copied.size() == 3);
    assert(original.size() == 2);
}

void test_copy_assignment() {
    SequentialQueue original;
    original.enqueue(10);
    original.enqueue(20);

    SequentialQueue assigned;
    assigned.enqueue(99);

    assigned = original;

    assert(assigned.size() == 2);
    assert(assigned.front() == 10);

    assigned.dequeue();

    assert(assigned.front() == 20);
    assert(original.front() == 10);
}

void test_self_assignment() {
    SequentialQueue queue;
    queue.enqueue(42);

    SequentialQueue& same_queue = queue;
    queue = same_queue;
    assert(queue.size() == 1);
    assert(queue.front() == 42);
}

int main() {
    test_empty_queue();
    test_enqueue_and_dequeue();
    test_copy_constructor();
    test_copy_assignment();
    test_self_assignment();

    std::cout << "All SequentialQueue tests passed!\n";
    return 0;
}
