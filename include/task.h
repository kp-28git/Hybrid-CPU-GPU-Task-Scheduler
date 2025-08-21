#ifndef TASK_H
#define TASK_H

#include <functional>
#include <string>
#include <map>
#include <variant>
#include "metrics.h"

enum class operation {
    MATRIX_MULTIPLY,
    VECTOR_ADD,
    SORTING,
    GAUSSIAN_BLUR
};

enum class executionUnit
{
    CPU,
    GPU
};

inline std::map<operation, std::string> operationNames = {
    {operation::MATRIX_MULTIPLY, "Matrix Multiplication"},
    {operation::VECTOR_ADD, "Vector Addition"},
    {operation::SORTING, "Sorting"},
    {operation::GAUSSIAN_BLUR, "Gaussian Blur"}
};

using taskArg = std::variant<int, size_t, std::string, std::vector<int>>;

class metrics;

class task : public metrics {
public:
    task() = default; 
    task(operation op, size_t dataSize);
    task(operation op, std::string fileName);

    operation op;
    taskArg arg;
    std::string nameTask;
    unsigned int computationSize;
    executionUnit executionunit;
    
    void runTask();
    void assignWork(std::function<void(taskArg)> fn);

private:
    size_t getComputationSize();
    std::function<void(taskArg)> work;
};

#endif // TASK_H
