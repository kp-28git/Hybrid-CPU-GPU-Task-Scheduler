#include "metrics.h"

// metrics::metrics(const std::string& taskName) : taskName(taskName) {}

void metrics::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void metrics::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    std::cout << "[" << taskName << "] Elapsed time: " << ms << " ms\n";
}

double metrics::elapsedMs() const {
    return std::chrono::duration<double, std::milli>(endTime - startTime).count();
}
