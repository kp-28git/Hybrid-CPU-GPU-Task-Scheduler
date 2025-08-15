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
    void matrixMultiplyCPU(std::vector<std::vector<int>> &A,
                                      std::vector<std::vector<int>> &B,
                                      std::vector<std::vector<int>> &C,
                                      size_t N);

private:
    static Image readPPM(const std::string &filename);
    static void writePPM(const std::string &filename, const Image &img);

    static void printMatrix(std::vector<std::vector<int>> &matrix);
    static void generateRandomMatrix(std::vector<std::vector<int>> &matrix);
};

#endif // CPU_TASKS_H
