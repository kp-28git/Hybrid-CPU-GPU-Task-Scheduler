#include "gpu_tasks.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

gpu_tasks::gpu_tasks(size_t N) : N(N), matrixA(N*N), matrixB(N*N), matrixC(N*N, 0.0f) {
    // Fill matrices with example values
    for (size_t i = 0; i < N*N; ++i) {
        matrixA[i] = static_cast<float>(i + 1);
        matrixB[i] = static_cast<float>((i % N) + 1);
    }

    initOpenCL();
}

std::string gpu_tasks::loadKernel(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open kernel file");
    return std::string((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
}

void gpu_tasks::initOpenCL() {
    cl_int err;

    // Platform
    err = clGetPlatformIDs(1, &platform, nullptr);

    // Device (first GPU)
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);

    // Context & Queue
    context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    cl_queue_properties props[] = { 0 }; // empty properties
    queue = clCreateCommandQueueWithProperties(context, device, props, &err);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create OpenCL command queue");
    }

    // Load and build kernel
    std::string src = loadKernel("kernels/matmul.cl");
    const char* srcPtr = src.c_str();
    size_t srcLen = src.size();
    program = clCreateProgramWithSource(context, 1, &srcPtr, &srcLen, &err);
    err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
        std::vector<char> log(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log.data(), nullptr);
        std::cerr << "Build log:\n" << log.data() << std::endl;
        throw std::runtime_error("Failed to build OpenCL program");
    }

    kernel = clCreateKernel(program, "matMul", &err);
}

void gpu_tasks::matrixMultiplyGPU() {
    cl_int err;

    // Buffers
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(float)*N*N, matrixA.data(), &err);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(float)*N*N, matrixB.data(), &err);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                 sizeof(float)*N*N, nullptr, &err);

    // Kernel args
    clSetKernelArg(kernel, 0, sizeof(int), &N);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufA);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufB);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &bufC);

    // Launch kernel
    size_t global[2] = { N, N };
    err = clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global, nullptr, 0, nullptr, nullptr);
    clFinish(queue);

    // Read result
    err = clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, sizeof(float)*N*N, matrixC.data(), 0, nullptr, nullptr);

    // Cleanup buffers
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);

    std::cout << "[GPU] Matrix multiplication finished.\n";
}

void gpu_tasks::printResult() const {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << matrixC[i*N + j] << "\t";
        }
        std::cout << "\n";
    }
}

void gpu_tasks::cleanupOpenCL() {
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}
