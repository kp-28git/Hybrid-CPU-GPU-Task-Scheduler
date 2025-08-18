#include <iostream>
#include <vector>
#include <string>
#include "cpu_tasks.h"
#include "gpu_tasks.h"
#include "metrics.h"
#include "scheduler.h"
#include "task.h"

int main() {

    cpu_tasks cpu;
    gpu_tasks gpu;
    metrics timer;
    timer.start();

    // Create scheduler with a policy
    scheduler scheduler(Policy::SOLE_GPU, 5000);

   std::vector<int> N = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    task t1("MatrixMultiply-2", N[0],
        [&]() { cpu.sortingCPU(N[0]); },
        [&]() {gpu.sortingGPU(N[0]); });

    task t2("MatrixMultiply-4", N[1],
        [&]() { cpu.sortingCPU(N[1]); },
        [&]() {gpu.sortingGPU(N[1]); });

    task t3("MatrixMultiply-8", N[2],
        [&]() { cpu.sortingCPU(N[2]); },
        [&]() {gpu.sortingGPU(N[2]); });
    
    task t4("MatrixMultiply-16", N[3],
        [&]() { cpu.sortingCPU(N[3]); },
        [&]() {gpu.sortingGPU(N[3]); });

    task t5("MatrixMultiply-32", N[4],
        [&]() { cpu.sortingCPU(N[4]); },
        [&]() {gpu.sortingGPU(N[4]); });

    task t6("MatrixMultiply-64", N[5],
        [&]() { cpu.sortingCPU(N[5]); },
        [&]() {gpu.sortingGPU(N[5]); });

    task t7("MatrixMultiply-128", N[6],
        [&]() { cpu.sortingCPU(N[6]); },
        [&]() {gpu.sortingGPU(N[6]); });

    task t8("MatrixMultiply-256", N[7],
        [&]() { cpu.sortingCPU(N[7]); },
        [&]() {gpu.sortingGPU(N[7]); });

    task t9("MatrixMultiply-512", N[8],
        [&]() { cpu.sortingCPU(N[8]); },
        [&]() {gpu.sortingGPU(N[8]); });

    task t10("MatrixMultiply-1024", N[9],
        [&]() { cpu.sortingCPU(N[9]); },
        [&]() {gpu.sortingGPU(N[9]); });

    scheduler.addTask(t1);
    scheduler.addTask(t2);
    scheduler.addTask(t3);
    scheduler.addTask(t4);
    scheduler.addTask(t5);
    scheduler.addTask(t6);
    scheduler.addTask(t7);
    scheduler.addTask(t8);
    scheduler.addTask(t9);
    scheduler.addTask(t10);
    scheduler.run();
    
    timer.stop();
    std::cout << "\n\nTotal execution time: " << timer.elapsedMs() << "\n";
    return 0;
}
