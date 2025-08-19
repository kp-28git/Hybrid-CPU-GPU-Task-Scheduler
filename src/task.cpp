#include "task.h"
#include <iostream>

task::task(operation op, size_t dataSize)
    : op(op), dataSize(dataSize) {}

const std::string& task::getName() const {
    return name;
}

size_t task::getDataSize() const {
    return dataSize;
}

void task::runTask() {
    std::cout << "Running task: \n";
    if (work) work(dataSize);
}

void task::assignWork(std::function<void(size_t N)> workFunc) {
    work = workFunc;
}