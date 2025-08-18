#include "scheduler.h"
#include <iostream>

scheduler::scheduler(Policy policy, size_t threshold)
    : policy(policy), threshold(threshold) {}

void scheduler::addTask(const task& task) {
    std::lock_guard<std::mutex> lock(mtx);
    tasks.push(task);
}

void scheduler::run() {
    while (!tasks.empty()) {
        task task = tasks.front();
        tasks.pop();

        if (policy == Policy::SOLE_CPU) {
            std::cout << "[CPU] Executing: " << task.getName() << "\n";
            task.runCPU();
        }
        else if (policy == Policy::SOLE_GPU) {
            std::cout << "[GPU] Executing: " << task.getName() << "\n";
            task.runGPU();
        }
        else if (policy == Policy::ROUND_ROBIN) {
            if (cpuTurn) {
                std::cout << "[CPU] Executing: " << task.getName() << "\n";
                task.runCPU();
            } else {
                std::cout << "[GPU] Executing: " << task.getName() << "\n";
                task.runGPU();
            }
            cpuTurn = !cpuTurn; // alternate
        }
        else if (policy == Policy::DATA_SIZE_BASED) {
            if (task.getDataSize() < threshold) {
                std::cout << "[CPU] Executing: " << task.getName() << "\n";
                task.runCPU();
            } else {
                std::cout << "[GPU] Executing: " << task.getName() << "\n";
                task.runGPU();
            }
        }
    }
}
