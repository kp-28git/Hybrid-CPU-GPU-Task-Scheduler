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

    //Vector Addition
    void vectorAddCPU(size_t N);

private:

    static Image readPPM(const std::string &filename);
    static void writePPM(const std::string &filename, const Image &img);

    static void printVector(const std::vector<int> &vec);
    static std::vector<int> generateRandomVector(size_t N);
};

#endif // CPU_TASKS_H
