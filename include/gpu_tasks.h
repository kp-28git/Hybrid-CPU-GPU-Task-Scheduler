#ifndef GPU_TASKS_H
#define GPU_TASKS_H

#define CL_TARGET_OPENCL_VERSION 300
#include <vector>
#include <CL/cl.h>
#include <string>

class gpu_tasks {
public:
    // Constructor: initialize NxN matrices
    gpu_tasks(size_t N);

    // Perform GPU matrix multiplication
    void matrixMultiplyGPU();

    // Print result matrix
    void printResult() const;

private:
    size_t N;
    std::vector<float> matrixA;
    std::vector<float> matrixB;
    std::vector<float> matrixC;

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
};

#endif // GPU_TASKS_H
