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

void cpu_tasks::printMatrix(std::vector<std::vector<int>> &matrix) {
    size_t N = matrix.size();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> cpu_tasks::generateRandomMatrix(size_t N) {
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

void cpu_tasks::matrixMultiplyCPU(size_t N) {
    matrixA = generateRandomMatrix(N);
    std::cout << "Matrix A:\n";
    printMatrix(matrixA);
    matrixB = generateRandomMatrix(N);                        
    std::cout << "Matrix B:\n";
    printMatrix(matrixB);

    metrics timer;
    timer.start();

    matrixC.resize(N, std::vector<int>(N, 0));
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            matrixC[i][j] = 0;
            for (size_t k = 0; k < N; ++k) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    timer.stop();

    std::cout << "[CPU] Matrix multiplication result:\n";
    printMatrix(matrixC);
}
