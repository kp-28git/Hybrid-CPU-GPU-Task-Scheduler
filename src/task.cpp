#include "task.h"
#include <iostream>

task::task(operation op, size_t dataSize)
    : op(op), arg(dataSize) {
        nameTask = operationNames[op] + "-" + std::to_string(dataSize);
    }

task::task(operation op, std::string fileName)
    : op(op),arg(fileName) {
        nameTask = operationNames[op] + "-" + fileName;
    }

size_t task::getComputationSize() {
;
    if (op == operation::MATRIX_MULTIPLY || op == operation::GAUSSIAN_BLUR) {
        computationSize *= computationSize;
    }
    else if (op == operation::SORTING || op == operation::VECTOR_ADD) {
        computationSize = computationSize;
    }
    return computationSize;
}

void task::runTask() {
    std::string ex = (executionunit == executionUnit::CPU) ? " [CPU]" : " [GPU]";
    std::cout << "Running task: "<< nameTask <<ex <<"\n";
    metrics::start();
    if (work) work(arg);
    metrics::stop();
}

void task::assignWork(std::function<void(taskArg)> fn){
        work = fn;
}