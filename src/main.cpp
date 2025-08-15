#include <iostream>
#include <vector>
#include <string>
#include "cpu_tasks.h"

int main() {
    std::cout << "=== CPU Tasks Demo ===" << std::endl;

    // 1. Gaussian blur (test with your image)
    // std::string inputImagePath = "input.png";
    // std::string outputImagePath = "output.png";
    // cpu_tasks::gaussianBlurCPU(inputImagePath,outputImagePath);
    // std::cout << "Gaussian blur done. Output saved as blurred.ppm\n";

    // 2. Matrix multiplication
    size_t N = 3; // Size of the matrices
    std::vector<std::vector<int>> matrixA(N, std::vector<int>(N));
    std::vector<std::vector<int>> matrixB(N, std::vector<int>(N));
    std::vector<std::vector<int>> matrixC(N, std::vector<int>(N));


    cpu_tasks cpuObj;
    cpuObj.matrixMultiplyCPU(matrixA, matrixB, matrixC, N);

    return 0;
}
