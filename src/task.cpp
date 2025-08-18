#include "task.h"

task::task(const std::string& name, size_t dataSize,
           std::function<void()> cpuFunc,
           std::function<void()> gpuFunc)
    : name(name), dataSize(dataSize*dataSize),
      cpuWork(cpuFunc), gpuWork(gpuFunc) {}

const std::string& task::getName() const {
    return name;
}

size_t task::getDataSize() const {
    return dataSize;
}

void task::runCPU() const {
    if (cpuWork) cpuWork();
}

void task::runGPU() const {
    if (gpuWork) gpuWork();
}
