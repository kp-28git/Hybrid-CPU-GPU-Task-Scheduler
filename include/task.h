#ifndef TASK_H
#define TASK_H

#include <functional>
#include <string>

class task {
public:
    task(const std::string& name, size_t dataSize,
         std::function<void()> cpuFunc,
         std::function<void()> gpuFunc);

    const std::string& getName() const;
    size_t getDataSize() const;

    void runCPU() const;
    void runGPU() const;

private:
    std::string name;
    size_t dataSize;
    std::function<void()> cpuWork;
    std::function<void()> gpuWork;
};

#endif // TASK_H
