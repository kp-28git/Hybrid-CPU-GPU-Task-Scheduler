#ifndef GPU_TASKS_H
#define GPU_TASKS_H

#define CL_TARGET_OPENCL_VERSION 300
#include <vector>
#include <CL/cl.h>
#include <string>

class gpu_tasks {
public:
    gpu_tasks();

    // Perform GPU matrix multiplication
    void matrixMultiplyGPU(size_t N);

private:
    size_t N;
    std::vector<std::vector<int>> matrixA;
    std::vector<std::vector<int>> matrixB;
    std::vector<std::vector<int>> matrixC;

    // OpenCL objects
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    void initOpenCL();
    std::string loadKernel(const std::string &filename);
    void cleanupOpenCL();

    // Print result matrix
    void printMatrix(std::vector<std::vector<int>> &matrix);

    // Generate a random NxN matrix
    std::vector<std::vector<int>> generateRandomMatrix(size_t N);
};

#endif // GPU_TASKS_H
