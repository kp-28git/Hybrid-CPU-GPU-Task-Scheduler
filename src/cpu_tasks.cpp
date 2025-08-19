#include "cpu_tasks.h"
#include "metrics.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <random>

cpu_tasks::Image cpu_tasks::readPPM(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file " + filename);

    std::string format;
    int width, height, maxval;
    file >> format >> width >> height >> maxval;
    file.get(); // skip single whitespace

    Image img{width, height, std::vector<unsigned char>(width * height * 3)};
    file.read(reinterpret_cast<char*>(img.data.data()), img.data.size());
    return img;
}

void cpu_tasks::writePPM(const std::string &filename, const Image &img) {
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << img.width << " " << img.height << "\n255\n";
    file.write(reinterpret_cast<const char*>(img.data.data()), img.data.size());
}

void cpu_tasks::gaussianBlurCPU(const std::string &imagePath, const std::string &outputPath) {
    Image img = readPPM(imagePath);
    Image output = img;
    int w = img.width;
    int h = img.height;

    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    int kernelSum = 16;

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            for (int c = 0; c < 3; ++c) {
                int sum = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int idx = ((y + ky) * w + (x + kx)) * 3 + c;
                        sum += img.data[idx] * kernel[ky + 1][kx + 1];
                    }
                }
                output.data[(y * w + x) * 3 + c] = sum / kernelSum;
            }
        }
    }

    writePPM(outputPath, output);
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
