#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include "cpu_tasks.h"
#include "gpu_tasks.h"
#include <queue>
#include <mutex>
#include <thread>
#include <map>
#include <functional>

enum class Policy {
    SOLE_CPU,
    SOLE_GPU,
    ROUND_ROBIN,
    DATA_SIZE_BASED
};

inline std::map<Policy, std::string> policyNames = {
    {Policy::SOLE_CPU, "Sole CPU"},
    {Policy::SOLE_GPU, "Sole GPU"},
    {Policy::ROUND_ROBIN, "Round Robin"},
    {Policy::DATA_SIZE_BASED, "Data Size Based"}
};

class scheduler {
public:
    scheduler(Policy policy, size_t threshold = 5000);

    void addTask(task& task);
    void run();

private:
    void runCPU();
    void runGPU();

    cpu_tasks cpu;
    gpu_tasks gpu;

    std::map<operation, std::function<void(taskArg)>> cpuOperations;
    std::map<operation, std::function<void(taskArg)>> gpuOperations;

    std::queue<task> cpuTasks;
    std::queue<task> gpuTasks;
    
    std::mutex cpuMtx;
    std::mutex gpuMtx;

    Policy policy;
    size_t threshold;
    bool cpuTurn = true; // For round-robin
};

#endif // SCHEDULER_H
