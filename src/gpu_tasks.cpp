#include "gpu_tasks.h"
#include "metrics.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <random>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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
    if (err != CL_SUCCESS) throw std::runtime_error("clGetPlatformIDs failed");

    // Device (first GPU)
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (err != CL_SUCCESS) throw std::runtime_error("clGetDeviceIDs failed");

    // Context & Queue
    context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    if (!context || err != CL_SUCCESS) throw std::runtime_error("clCreateContext failed");

    cl_queue_properties props[] = { 0 };
    queue = clCreateCommandQueueWithProperties(context, device, props, &err);
    if (!queue || err != CL_SUCCESS) throw std::runtime_error("clCreateCommandQueueWithProperties failed");
}

cl_kernel gpu_tasks::buildKernelFromFile(cl_context ctx, cl_device_id dev,
                                     const std::string& path, const char* kernelName,
                                     cl_program* outProgram /* to release later */)
{
    cl_int err;
    // load source
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open kernel file: " + path);
    std::string src((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    const char* sp = src.c_str();
    size_t sl = src.size();

    // create + build program
    cl_program prog = clCreateProgramWithSource(ctx, 1, &sp, &sl, &err);
    if (!prog || err != CL_SUCCESS) throw std::runtime_error("clCreateProgramWithSource failed");

    err = clBuildProgram(prog, 1, &dev, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        size_t log_size = 0;
        clGetProgramBuildInfo(prog, dev, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
        std::vector<char> log(log_size);
        clGetProgramBuildInfo(prog, dev, CL_PROGRAM_BUILD_LOG, log_size, log.data(), nullptr);
        std::cerr << "Build log (" << path << "):\n" << log.data() << std::endl;
        clReleaseProgram(prog);
        throw std::runtime_error("clBuildProgram failed");
    }

    cl_kernel k = clCreateKernel(prog, kernelName, &err);
    if (!k || err != CL_SUCCESS) {
        clReleaseProgram(prog);
        throw std::runtime_error(std::string("clCreateKernel failed for ") + kernelName);
    }

    if (outProgram) *outProgram = prog; // return program so caller can release
    return k;
}

void gpu_tasks::printVector(const std::vector<int> &vec) {
    for (const auto &val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

std::vector<int> gpu_tasks::generateRandomVector(size_t N) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    std::vector<int> vec(N);
    for (size_t i = 0; i < N; ++i) {
        vec[i] = dist(gen);
    }
    return vec;
}

void gpu_tasks::matrixMultiplyGPU(size_t N) {
    auto matA = generateRandomVector(N * N);
    auto matB = generateRandomVector(N * N);
    std::vector<int> matC(N * N, 0);

    // metrics timer;
    // timer.start();

    cl_int err;
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, sizeof(int) * N * N, matA.data(), &err);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, sizeof(int) * N * N, matB.data(), &err);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,                        sizeof(int) * N * N, nullptr, &err);

    // Load kernel
    cl_program prog = nullptr;
    cl_kernel kern = buildKernelFromFile(context, device, "kernels/matmul.cl", "matmul", &prog);

    // Set kernel args
    err  = clSetKernelArg(kern, 0, sizeof(cl_mem), &bufA);
    err |= clSetKernelArg(kern, 1, sizeof(cl_mem), &bufB);
    err |= clSetKernelArg(kern, 2, sizeof(cl_mem), &bufC);
    err |= clSetKernelArg(kern, 3, sizeof(int),    &N);
    if (err != CL_SUCCESS) throw std::runtime_error("clSetKernelArg failed (matmul)");

    // Global work size: 2D grid (N x N)
    size_t global[2] = { N, N };
    err = clEnqueueNDRangeKernel(queue, kern, 2, nullptr, global, nullptr, 0, nullptr, nullptr);
    if (err != CL_SUCCESS) throw std::runtime_error("clEnqueueNDRangeKernel failed (matmul)");
    clFinish(queue);

    // Read back result
    err = clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, sizeof(int) * N * N, matC.data(), 0, nullptr, nullptr);
    if (err != CL_SUCCESS) throw std::runtime_error("clEnqueueReadBuffer failed (matmul)");

    // Release
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseKernel(kern);
    clReleaseProgram(prog);

    // timer.stop();

    // std::cout << "[GPU] Result for matrix multiplication:\n";
    // printVector(matC);
}

void gpu_tasks::vectorAddGPU(size_t N) {
    auto vecA = generateRandomVector(N);
    auto vecB = generateRandomVector(N);
    std::vector<int> vecC(N, 0);

    // metrics timer;
    // timer.start();

    cl_int err;
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, sizeof(int)*N, vecA.data(), &err);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, sizeof(int)*N, vecB.data(), &err);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,                        sizeof(int)*N, nullptr,     &err);

    cl_program prog = nullptr;
    cl_kernel  kern = buildKernelFromFile(context, device, "kernels/vecadd.cl", "vectoradd", &prog);

    // Set kernel args
    err  = clSetKernelArg(kern, 0, sizeof(cl_mem), &bufA);
    err |= clSetKernelArg(kern, 1, sizeof(cl_mem), &bufB);
    err |= clSetKernelArg(kern, 2, sizeof(cl_mem), &bufC);
    err |= clSetKernelArg(kern, 3, sizeof(int),    &N);
    if (err != CL_SUCCESS) throw std::runtime_error("clSetKernelArg failed (vectoradd)");

    size_t global[1] = { N };
    err = clEnqueueNDRangeKernel(queue, kern, 1, nullptr, global, nullptr, 0, nullptr, nullptr);
    if (err != CL_SUCCESS) throw std::runtime_error("clEnqueueNDRangeKernel failed (vectoradd)");
    clFinish(queue);

    err = clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, sizeof(int)*N, vecC.data(), 0, nullptr, nullptr);
    if (err != CL_SUCCESS) throw std::runtime_error("clEnqueueReadBuffer failed (vectoradd)");

    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseKernel(kern);
    clReleaseProgram(prog);

    // timer.stop();

    // std::cout << "[GPU] Result for vector addition:\n";
    // printVector(vecC);
}

void gpu_tasks::sortingGPU(size_t N) {
    auto vec = generateRandomVector(N);

    // metrics timer;
    // timer.start();

    cl_int err;
    cl_mem buf = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                sizeof(int) * N, vec.data(), &err);

    cl_program prog = nullptr;
    cl_kernel kern = buildKernelFromFile(context, device, "kernels/sort.cl", "sort", &prog);

    size_t global[1] = { N / 2 };

    for (int phase = 0; phase < N; phase++) {
        int parity = phase % 2;
        err  = clSetKernelArg(kern, 0, sizeof(cl_mem), &buf);
        err |= clSetKernelArg(kern, 1, sizeof(int), &parity);
        err |= clSetKernelArg(kern, 2, sizeof(int), &N);
        if (err != CL_SUCCESS)
            throw std::runtime_error("clSetKernelArg failed (sorting)");

        err = clEnqueueNDRangeKernel(queue, kern, 1, nullptr, global, nullptr,
                                     0, nullptr, nullptr);
        if (err != CL_SUCCESS)
            throw std::runtime_error("clEnqueueNDRangeKernel failed (sorting)");
        clFinish(queue);
    }

    // Read back
    err = clEnqueueReadBuffer(queue, buf, CL_TRUE, 0,
                              sizeof(int) * N, vec.data(),
                              0, nullptr, nullptr);
    if (err != CL_SUCCESS)
        throw std::runtime_error("clEnqueueReadBuffer failed (sorting)");

    clReleaseMemObject(buf);

    // timer.stop();
    
    // std::cout << "[GPU] Result for sorting : ";
    // printVector(vec);
}


void gpu_tasks::gaussianBlurGPU(std::string &fileName) {

    std::string inputPath  = "input_img/" + fileName;
    std::string outputFolder = "output_img";
    std::filesystem::create_directories(outputFolder);
    std::string outputPath = outputFolder + "/out_" + fileName;
    cv::Mat img = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);
    if (img.empty()) throw std::runtime_error("Failed to load image");

    int width  = img.cols;
    int height = img.rows;

    std::vector<unsigned char> input(img.data, img.data + img.total());
    std::vector<unsigned char> output(img.total(), 0);

    cl_int err;
    cl_mem bufInput  = clCreateBuffer(context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, sizeof(unsigned char) * input.size(), input.data(), &err);
    cl_mem bufOutput = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * output.size(), nullptr, &err);

    cl_program prog = nullptr;
    cl_kernel kern = buildKernelFromFile(context, device, "kernels/gaussianblur.cl", "gaussianblur", &prog);

    err  = clSetKernelArg(kern, 0, sizeof(cl_mem), &bufInput);
    err |= clSetKernelArg(kern, 1, sizeof(cl_mem), &bufOutput);
    err |= clSetKernelArg(kern, 2, sizeof(int), &width);
    err |= clSetKernelArg(kern, 3, sizeof(int), &height);
    if (err != CL_SUCCESS) throw std::runtime_error("clSetKernelArg failed");

    size_t global[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
    err = clEnqueueNDRangeKernel(queue, kern, 2, nullptr, global, nullptr, 0, nullptr, nullptr);
    clFinish(queue);

    err = clEnqueueReadBuffer(queue, bufOutput, CL_TRUE, 0, sizeof(unsigned char) * output.size(), output.data(), 0, nullptr, nullptr);

    clReleaseMemObject(bufInput);
    clReleaseMemObject(bufOutput);
    clReleaseKernel(kern);
    clReleaseProgram(prog);

    cv::Mat result(height, width, CV_8UC1, output.data());
    cv::imwrite(outputPath, result);
}


void gpu_tasks::cleanupOpenCL() {
    clReleaseDevice(device);
    clReleaseContext(context);
    clReleaseCommandQueue(queue);
}

gpu_tasks::~gpu_tasks() {
    cleanupOpenCL();
}