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
    size_t N = 5; // Size of the matrices



    cpu_tasks cpuObj;
    cpuObj.matrixMultiplyCPU(N);

    return 0;
}
