cmake_minimum_required(VERSION 3.10)
project(HybridSchedulerCPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCL REQUIRED)

# Include directories
include_directories(include)               # your project headers
include_directories(${OpenCL_INCLUDE_DIRS}) # OpenCL headers

# Source files
set(SOURCES
    src/main.cpp
    src/cpu_tasks.cpp
    src/gpu_tasks.cpp
    src/metrics.cpp
)

# Create executable
add_executable(hybrid_scheduler_cpu ${SOURCES})

# Link OpenCL library
target_link_libraries(hybrid_scheduler_cpu ${OpenCL_LIBRARIES})

# Copy kernel file(s) to build directory
file(COPY ${CMAKE_SOURCE_DIR}/kernels/matmul.cl DESTINATION ${CMAKE_BINARY_DIR}/kernels)