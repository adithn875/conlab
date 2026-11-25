#include <cassert>
#include <iostream>
#include <stdexcept>
#include "sequential_queue.hpp"

int main() {
    // 1. Empty queue
    SequentialQueue queue;
    assert(queue.is_empty());
    assert(queue.size() == 0);

    // 2. Enqueue and FIFO behaviour
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    assert(queue.size() == 3);
    assert(queue.front() == 10);

    // 3. Dequeue
    queue.dequeue();
    assert(queue.front() == 20);
    assert(queue.size() == 2);

    // 4. Copy constructor
    SequentialQueue copied(queue);
    assert(copied.size() == 2);
    assert(copied.front() == 20);

    // 5. Independent copies
    queue.dequeue();
    assert(queue.front() == 30);
    assert(copied.front() == 20);

    // 6. Copy assignment
    SequentialQueue assigned;
    assigned.enqueue(99);
    assigned = copied;

    assert(assigned.size() == 2);
    assert(assigned.front() == 20);

    // 7. Self-assignment
    assigned = assigned;
    assert(assigned.size() == 2);
    assert(assigned.front() == 20);

    // 8. Empty queue dequeue
    SequentialQueue empty;
    empty.dequeue();
    assert(empty.is_empty());
    assert(empty.size() == 0);

    // 9. Empty queue front
    try {
        empty.front();
        assert(false && "Expected out_of_range exception");
    }
    catch (const std::out_of_range&) {
        // Expected
    }

    // 10. Remove all elements
    while (!assigned.is_empty()) {
        assigned.dequeue();
    }

    assert(assigned.is_empty());
    assert(assigned.size() == 0);

    std::cout << "All sequential queue tests passed!" << std::endl;

    return 0;
}