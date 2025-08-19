#include "task.h"
#include <iostream>

task::task(operation op, size_t dataSize)
    : op(op), dataSize(dataSize) {
        name = operationNames[op] + "-" + std::to_string(dataSize);
    }

size_t task::getComputationSize() const {
    size_t computationSize = dataSize;
    if (op == operation::MATRIX_MULTIPLY || op == operation::VECTOR_ADD) {
        computationSize *= dataSize;
    }
    else if (op == operation::SORTING) {
        computationSize = dataSize;
    }
    return computationSize;
}

void task::runTask() {
    std::string ex = (executionunit == executionUnit::CPU) ? " [CPU]" : " [GPU]";
    std::cout << "Running task: "<< name <<ex <<"\n";
    metrics::start();
    if (work) work(dataSize);
    metrics::stop();
}

void task::assignWork(std::function<void(size_t N)> workFunc) {
    work = workFunc;
}