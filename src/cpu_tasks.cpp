#include "cpu_tasks.h"
#include "metrics.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdexcept>
#include <random>
#include <filesystem>


#define OPENCV_COMPUTATION   1

void cpu_tasks::gaussianBlurCPU(std::string &fileName) {

    std::string inputPath  = "input_img/" + fileName;
    std::string outputFolder = "output_img";
    std::filesystem::create_directories(outputFolder);
    std::string outputPath = outputFolder + "/out_" + fileName;

    cv::Mat input = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);
    if (input.empty()) {
        std::cerr << "Error: Could not open input image: " << inputPath << std::endl;
        return;
    }

    cv::Mat output = input.clone(); // same size as input

    // 5x5 Gaussian kernel
    float kernel[5][5] = {
        {1/256.f,  4/256.f,  6/256.f,  4/256.f, 1/256.f},
        {4/256.f, 16/256.f, 24/256.f, 16/256.f, 4/256.f},
        {6/256.f, 24/256.f, 36/256.f, 24/256.f, 6/256.f},
        {4/256.f, 16/256.f, 24/256.f, 16/256.f, 4/256.f},
        {1/256.f,  4/256.f,  6/256.f,  4/256.f, 1/256.f}
    };

    int rows = input.rows;
    int cols = input.cols;

    if (OPENCV_COMPUTATION) {
    cv::Mat kernelData(5, 5, CV_32F, kernel);
    cv::filter2D(input, output, -1, kernelData);
    } 
    else {
        // Apply kernel using loops
        for (int i = 2; i < rows - 2; ++i) {
            for (int j = 2; j < cols - 2; ++j) {
                float sum = 0.0f;

                // Loop over the kernel
                for (int ki = -2; ki <= 2; ++ki) {
                    for (int kj = -2; kj <= 2; ++kj) {
                        sum += kernel[ki + 2][kj + 2] * input.at<uchar>(i + ki, j + kj);
                    }
                }

                output.at<uchar>(i, j) = static_cast<uchar>(sum);
            }
        }
    }



    
    if (!cv::imwrite(outputPath, output)) {
        std::cerr << "Error: Could not save output image: " << outputPath << std::endl;
    }
}


void cpu_tasks::printVector(const std::vector<int> &vec) {
    for (const auto &val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

std::vector<int> cpu_tasks::generateRandomVector(size_t N) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    std::vector<int> vec(N, 0);
    for (size_t i = 0; i < N; ++i) {
        vec[i] = dist(gen);
    }
    return vec;
}

void cpu_tasks::matrixMultiplyCPU(size_t N) {
    
    auto matrixA = generateRandomVector(N * N);
    auto matrixB = generateRandomVector(N * N);
    std::vector<int> matrixC(N * N, 0);

    // metrics timer;
    // timer.start();

    auto idx = [N](size_t row, size_t col) {
    return row * N + col;
    };

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            int sum = 0;
            for (size_t k = 0; k < N; k++) {
                sum += matrixA[idx(i, k)] * matrixB[idx(k, j)];
            }
            matrixC[idx(i, j)] = sum;
        }
    }

    // timer.stop();

    // std::cout << "[CPU] Result for matrix multiplication : ";
    // printVector(matrixC);
}

void cpu_tasks::vectorAddCPU(size_t N) {
    auto vecA = generateRandomVector(N);
    auto vecB = generateRandomVector(N);
    std::vector<int> vecC(N, 0);

    // metrics timer;
    // timer.start();

    for (size_t i = 0; i < vecC.size(); i++) {
        vecC[i] = vecA[i] + vecB[i];
    }

    // timer.stop();

    // std::cout << "[CPU] Result for vector addition : ";
    // printVector(vecC);
}

void cpu_tasks::sortingCPU(size_t N) {
    auto vec = generateRandomVector(N);
    
    // metrics timer;
    // timer.start();

    bool sorted = false;
    while (!sorted) {
        sorted = true;

        // Odd phase
        for (size_t i = 1; i + 1 < N; i += 2) {
            if (vec[i] > vec[i + 1]) {
                std::swap(vec[i], vec[i + 1]);
                sorted = false;
            }
        }

        // Even phase
        for (size_t i = 0; i + 1 < N; i += 2) {
            if (vec[i] > vec[i + 1]) {
                std::swap(vec[i], vec[i + 1]);
                sorted = false;
            }
        }
    }

    // timer.stop();

    // std::cout << "[CPU] Result for sorting : ";
    // printVector(vec);
}
