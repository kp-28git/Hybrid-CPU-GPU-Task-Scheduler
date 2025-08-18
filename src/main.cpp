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
    scheduler scheduler(Policy::DATA_SIZE_BASED, 2000);

   std::vector<int> N = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    task t1("MatrixMultiply-4", N[0],
        [&]() { cpu.matrixMultiplyCPU(N[0]); },
        [&]() { gpu.matrixMultiplyGPU(N[0]); });

    task t2("MatrixMultiply-8", N[1],
        [&]() { cpu.matrixMultiplyCPU(N[1]); },
        [&]() { gpu.matrixMultiplyGPU(N[1]); });   

    task t3("MatrixMultiply-16", N[2],
        [&]() { cpu.matrixMultiplyCPU(N[2]); },
        [&]() { gpu.matrixMultiplyGPU(N[2]); });

    task t4("MatrixMultiply-32", N[3],
        [&]() { cpu.matrixMultiplyCPU(N[3]); },
        [&]() { gpu.matrixMultiplyGPU(N[3]); });

    task t5("MatrixMultiply-64", N[4],
        [&]() { cpu.matrixMultiplyCPU(N[4]); },
        [&]() { gpu.matrixMultiplyGPU(N[4]); });

    task t6("MatrixMultiply-128", N[5],
        [&]() { cpu.matrixMultiplyCPU(N[5]); },
        [&]() { gpu.matrixMultiplyGPU(N[5]); });

    task t7("MatrixMultiply-256", N[6],
        [&]() { cpu.matrixMultiplyCPU(N[6]); },
        [&]() { gpu.matrixMultiplyGPU(N[6]); });

    task t8("MatrixMultiply-512", N[7],
        [&]() { cpu.matrixMultiplyCPU(N[7]); },
        [&]() { gpu.matrixMultiplyGPU(N[7]); });

    task t9("MatrixMultiply-1024", N[8],
        [&]() { cpu.matrixMultiplyCPU(N[8]); },
        [&]() { gpu.matrixMultiplyGPU(N[8]); });

    scheduler.addTask(t1);
    scheduler.addTask(t2);
    scheduler.addTask(t3);
    scheduler.addTask(t4);
    scheduler.addTask(t5);
    scheduler.addTask(t6);
    scheduler.addTask(t7);
    scheduler.addTask(t8);
    scheduler.addTask(t9);

    scheduler.run();
    
    timer.stop();
    std::cout << "\n\nTotal execution time: " << timer.elapsedMs() << "\n";
    return 0;
}
