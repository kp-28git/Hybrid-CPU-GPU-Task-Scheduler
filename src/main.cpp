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
    scheduler scheduler(Policy::ROUND_ROBIN, 5000);

   std::vector<int> N = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    task t0(operation::SORTING, N[0]);
    scheduler.addTask(t0);

    task t1(operation::SORTING, N[1]);
    scheduler.addTask(t1);

    task t2(operation::SORTING, N[2]);
    scheduler.addTask(t2);

    task t3(operation::SORTING, N[3]);
    scheduler.addTask(t3);

    task t4(operation::MATRIX_MULTIPLY, N[4]);
    scheduler.addTask(t4);

    scheduler.run();
    
    timer.stop();
    std::cout << "\n\nTotal execution time: " << timer.elapsedMs() << "\n";
    return 0;
}
