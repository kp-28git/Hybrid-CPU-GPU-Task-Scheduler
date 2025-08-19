#ifndef TASK_H
#define TASK_H

#include <functional>
#include <string>

enum class operation {
    MATRIX_MULTIPLY,
    VECTOR_ADD,
    SORTING
};

class task {
public:
    task(operation op, size_t dataSize);

    const std::string& getName() const;
    size_t getDataSize() const;

    operation op;
    size_t dataSize;
    std::string name;
    
    void runTask();
    void assignWork(std::function<void(size_t N)> workFunc);

private:
    std::function<void(size_t N)> work;
};

#endif // TASK_H
