#ifndef TASK_H
#define TASK_H

#include <functional>
#include <string>
#include <map>
#include "metrics.h"

enum class operation {
    MATRIX_MULTIPLY,
    VECTOR_ADD,
    SORTING
};

enum class executionUnit
{
    CPU,
    GPU
};

inline std::map<operation, std::string> operationNames = {
    {operation::MATRIX_MULTIPLY, "Matrix Multiplication"},
    {operation::VECTOR_ADD, "Vector Addition"},
    {operation::SORTING, "Sorting"}
};

class metrics;

class task : public metrics {
public:
    task(operation op, size_t dataSize);

    std::string& getName();
    
    size_t getComputationSize() const;

    operation op;
    size_t dataSize;
    std::string name;
    executionUnit executionunit;
    
    void runTask();
    void assignWork(std::function<void(size_t N)> workFunc);

private:
    std::function<void(size_t N)> work;
};

#endif // TASK_H
