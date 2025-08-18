#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

enum class Policy {
    SOLE_CPU,
    SOLE_GPU,
    ROUND_ROBIN,
    DATA_SIZE_BASED
};

class scheduler {
public:
    scheduler(Policy policy, size_t threshold = 100);

    void addTask(const task& task);
    void run();

private:
    std::queue<task> tasks;
    std::mutex mtx;
    Policy policy;
    size_t threshold;
    bool cpuTurn = true; // For round-robin
};

#endif // SCHEDULER_H
