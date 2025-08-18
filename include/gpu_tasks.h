#ifndef GPU_TASKS_H
#define GPU_TASKS_H

#define CL_TARGET_OPENCL_VERSION 300
#include <vector>
#include <CL/cl.h>
#include <string>

class gpu_tasks {
public:
    gpu_tasks();
    ~gpu_tasks();

    // Perform GPU matrix multiplication
    void matrixMultiplyGPU(size_t N);

    // Perform GPU vector addition
    void vectorAddGPU(size_t N);

    // Perform GPU sorting (odd-even sort)
    void sortingGPU(size_t N);

private:
    size_t N;

    // OpenCL objects
    cl_platform_id platform = nullptr;
    cl_device_id device = nullptr;
    cl_context context = nullptr;
    cl_command_queue queue = nullptr;

    void initOpenCL();
    static cl_kernel buildKernelFromFile(cl_context ctx, cl_device_id dev,
                                         const std::string& path,
                                         const char* kernelName,
                                         cl_program* outProgram = nullptr);
    std::string loadKernel(const std::string &filename);
    void cleanupOpenCL();

    void printVector(const std::vector<int> &vec);

    std::vector<int> generateRandomVector(size_t N);
};

#endif // GPU_TASKS_H
