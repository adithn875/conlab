#include <iostream>
#include "sequential_queue.hpp"

int main() {
    SequentialQueue queue;

    std::cout << "Is queue empty? "
              << queue.is_empty() << std::endl;

    queue.enqueue(10);
    queue.dequeue();

    std::cout << "Is queue empty? "
              << queue.is_empty() << std::endl;

    queue.display();

    return 0;
}