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
    scheduler scheduler(Policy::ROUND_ROBIN, 5000);

    std::vector<std::string> fileName = {"cameraman.png", "tree.png", "buildings.png", "lion.png", "beach.png", "airport.png", "desert.png", "waterfall.png", "flower.png", "bridge.png"};
    std::vector<int> N = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    task t0(operation::GAUSSIAN_BLUR, fileName[0]);
    scheduler.addTask(t0);

    task t1(operation::GAUSSIAN_BLUR, fileName[1]);
    scheduler.addTask(t1);

    task t2(operation::GAUSSIAN_BLUR, fileName[2]);
    scheduler.addTask(t2);

    task t3(operation::GAUSSIAN_BLUR, fileName[3]);
    scheduler.addTask(t3);

    task t4(operation::GAUSSIAN_BLUR, fileName[4]);
    scheduler.addTask(t4);

    task t5(operation::GAUSSIAN_BLUR, fileName[5]);
    scheduler.addTask(t5);

    task t6(operation::GAUSSIAN_BLUR, fileName[6]);
    scheduler.addTask(t6);

    task t7(operation::GAUSSIAN_BLUR, fileName[7]);
    scheduler.addTask(t7);

    task t8(operation::GAUSSIAN_BLUR, fileName[8]);
    scheduler.addTask(t8);

    task t9(operation::GAUSSIAN_BLUR, fileName[9]);
    scheduler.addTask(t9);

    scheduler.run();

    // task t0(operation::VECTOR_ADD, N[0]);
    // scheduler.addTask(t0);

//     task t1(operation::VECTOR_ADD, N[1]);
//     scheduler.addTask(t1);

//     task t2(operation::VECTOR_ADD, N[2]);
//     scheduler.addTask(t2);

//     task t3(operation::VECTOR_ADD, N[3]);
//     scheduler.addTask(t3);

//     task t4(operation::VECTOR_ADD, N[4]);
//     scheduler.addTask(t4);

//     task t5(operation::VECTOR_ADD, N[5]);
//     scheduler.addTask(t5);

//     task t6(operation::VECTOR_ADD, N[6]);
//     scheduler.addTask(t6);

//     task t7(operation::VECTOR_ADD, N[7]);
//     scheduler.addTask(t7);

//     task t8(operation::VECTOR_ADD, N[8]);
//     scheduler.addTask(t8);

//     task t9(operation::VECTOR_ADD, N[9]);
//     scheduler.addTask(t9);

    // scheduler.run();
    
    std::cout << "\n\nTotal execution time : ";
    timer.stop();
    // std::cout << "\n\nTotal execution time: " << timer.elapsedMs() << "\n";
    return 0;
}
