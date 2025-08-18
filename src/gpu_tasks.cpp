#include "gpu_tasks.h"
#include "metrics.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <random>

gpu_tasks::gpu_tasks() {
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

void gpu_tasks::printMatrix(std::vector<std::vector<int>> &matrix) {
    size_t N = matrix.size();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> gpu_tasks::generateRandomMatrix(size_t N) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            matrix[i][j] = dist(gen);
        }
    }
    return matrix;
}

void gpu_tasks::matrixMultiplyGPU(size_t N) {
    
    std::vector<std::vector<int>> matrixA = generateRandomMatrix(N);
    std::vector<std::vector<int>> matrixB = generateRandomMatrix(N);
    std::vector<std::vector<int>> matrixC(N, std::vector<int>(N, 0));


    // std::cout << "Matrix A:\n";
    // printMatrix(matrixA);
    // std::cout << "Matrix B:\n";
    // printMatrix(matrixB);

    metrics timer;
    timer.start();

    // Flatten matrices for GPU
    std::vector<int> flatA(N * N);
    std::vector<int> flatB(N * N);
    std::vector<int> flatC(N * N, 0);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j) {
            flatA[i * N + j] = matrixA[i][j];
            flatB[i * N + j] = matrixB[i][j];
        }


    cl_int err;

    // Create GPU buffers
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(int) * N * N, flatA.data(), &err);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(int) * N * N, flatB.data(), &err);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                 sizeof(int) * N * N, nullptr, &err);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(int), &N);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufA);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufB);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &bufC);

    // Launch kernel
    size_t global[2] = { N, N };
    err = clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global, nullptr, 0, nullptr, nullptr);
    clFinish(queue);

    // Read back the result
    err = clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, sizeof(int) * N * N, flatC.data(), 0, nullptr, nullptr);

    // Release GPU buffers
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);

    // Convert flatC back to 2D matrixC
    matrixC.resize(N);
    for (size_t i = 0; i < N; ++i) {
        matrixC[i].resize(N);
        for (size_t j = 0; j < N; ++j)
            matrixC[i][j] = flatC[i * N + j];
    }

    timer.stop();
    // std::cout << "[GPU] Matrix multiplication result:\n";
    // printMatrix(matrixC);
}


void gpu_tasks::cleanupOpenCL() {
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

gpu_tasks::~gpu_tasks() {
    cleanupOpenCL();
}