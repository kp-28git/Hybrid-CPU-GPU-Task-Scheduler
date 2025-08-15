#ifndef CPU_TASKS_H
#define CPU_TASKS_H

#include <string>
#include <vector>

class cpu_tasks {
public:
    struct Image {
        int width;
        int height;
        std::vector<unsigned char> data;
    };

    // Image processing
    static void gaussianBlurCPU(const std::string &imagePath, const std::string &outputPath);

    // Matrix operations
    void matrixMultiplyCPU(size_t N);

private:
    std::vector<std::vector<int>> matrixA;
    std::vector<std::vector<int>> matrixB;
    std::vector<std::vector<int>> matrixC;

    static Image readPPM(const std::string &filename);
    static void writePPM(const std::string &filename, const Image &img);

    static void printMatrix(std::vector<std::vector<int>> &matrix);
    static std::vector<std::vector<int>> generateRandomMatrix(size_t N);
};

#endif // CPU_TASKS_H
