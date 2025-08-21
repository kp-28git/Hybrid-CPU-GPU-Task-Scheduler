#include "scheduler.h"
#include <iostream>

scheduler::scheduler(Policy policy, size_t threshold)
    : policy(policy), threshold(threshold) {

    std::cout << "Scheduler created with policy: " << policyNames[policy] << "\n";

    cpuOperations = {
        {operation::MATRIX_MULTIPLY, [&](taskArg arg) { cpu.matrixMultiplyCPU(std::get<size_t>(arg)); }},
        {operation::VECTOR_ADD, [&](taskArg arg) { cpu.vectorAddCPU(std::get<size_t>(arg)); }},
        {operation::SORTING, [&](taskArg arg) { cpu.sortingCPU(std::get<size_t>(arg)); }},
        {operation::GAUSSIAN_BLUR, [&](taskArg arg) { cpu.gaussianBlurCPU(std::get<std::string>(arg)); }}
    };
    gpuOperations = {
        {operation::MATRIX_MULTIPLY, [&](taskArg arg) { gpu.matrixMultiplyGPU(std::get<size_t>(arg)); }},
        {operation::VECTOR_ADD, [&](taskArg arg) { gpu.vectorAddGPU(std::get<size_t>(arg)); }},
        {operation::SORTING, [&](taskArg arg) { gpu.sortingGPU(std::get<size_t>(arg)); }},
        {operation::GAUSSIAN_BLUR, [&](taskArg arg) { gpu.gaussianBlurGPU(std::get<std::string>(arg)); }}
    };

}

void scheduler::addTask(task& task) {
    if(policy == Policy::SOLE_CPU) {
        task.assignWork(cpuOperations[task.op]);
        task.executionunit = executionUnit::CPU;
        cpuTasks.push(task);
    }
    else if(policy == Policy::SOLE_GPU) {
        task.assignWork(gpuOperations[task.op]);
        task.executionunit = executionUnit::GPU;
        gpuTasks.push(task);
    }
    else if(policy == Policy::ROUND_ROBIN) {
        if (cpuTurn) {
            task.assignWork(cpuOperations[task.op]);
            task.executionunit = executionUnit::CPU;
            cpuTasks.push(task);
        }   
        else {
            task.assignWork(gpuOperations[task.op]);
            task.executionunit = executionUnit::GPU;
            gpuTasks.push(task);
        }
        cpuTurn = !cpuTurn;
    }
    else if(policy == Policy::DATA_SIZE_BASED) {
        if (task.getComputationSize() < threshold) {
            task.assignWork(cpuOperations[task.op]);
            task.executionunit = executionUnit::CPU;
            cpuTasks.push(task);
        } else {
            task.assignWork(gpuOperations[task.op]);
            task.executionunit = executionUnit::GPU;
            gpuTasks.push(task);
        }
    }
}

void scheduler::runCPU() {
    while (true) {
        std::unique_lock<std::mutex> lock(cpuMtx);
        if (cpuTasks.empty()) break;
        task t = std::move(cpuTasks.front());
        cpuTasks.pop();
        lock.unlock();
        t.runTask();
    }
}

void scheduler::runGPU() {
    while (true) {
        std::unique_lock<std::mutex> lock(gpuMtx);
        if (gpuTasks.empty()) break;
        task t = std::move(gpuTasks.front());
        gpuTasks.pop();
        lock.unlock();
        t.runTask();
    }
}

void scheduler::run() {
    std::thread cpuThread(&scheduler::runCPU, this);
    std::thread gpuThread(&scheduler::runGPU, this);

    cpuThread.join();
    gpuThread.join();
}