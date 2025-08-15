cmake_minimum_required(VERSION 3.10)
project(HybridSchedulerCPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(hybrid_scheduler_cpu
    src/main.cpp
    src/cpu_tasks.cpp
)