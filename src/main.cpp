#include <iostream>
#include <vector>
#include <string>

#include "metrics.h"
#include "scheduler.h"
#include "task.h"

int main() {
    metrics timer;
    timer.start();

    // Create scheduler with a policy
    scheduler scheduler(Policy::SOLE_CPU, 5000);

   std::vector<int> N = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    task t0(operation::MATRIX_MULTIPLY, N[0]);
    scheduler.addTask(t0);

    task t1(operation::MATRIX_MULTIPLY, N[1]);
    scheduler.addTask(t1);

    task t2(operation::MATRIX_MULTIPLY, N[2]);
    scheduler.addTask(t2);

    task t3(operation::MATRIX_MULTIPLY, N[3]);
    scheduler.addTask(t3);

    task t4(operation::MATRIX_MULTIPLY, N[4]);
    scheduler.addTask(t4);

    task t5(operation::MATRIX_MULTIPLY, N[5]);
    scheduler.addTask(t5);

    task t6(operation::MATRIX_MULTIPLY, N[6]);
    scheduler.addTask(t6);

    task t7(operation::MATRIX_MULTIPLY, N[7]);
    scheduler.addTask(t7);

    task t8(operation::MATRIX_MULTIPLY, N[8]);
    scheduler.addTask(t8);

    task t9(operation::MATRIX_MULTIPLY, N[9]);
    scheduler.addTask(t9);

    scheduler.run();
    
    std::cout << "\n\nTotal execution time : ";
    timer.stop();
    // std::cout << "\n\nTotal execution time: " << timer.elapsedMs() << "\n";
    return 0;
}
