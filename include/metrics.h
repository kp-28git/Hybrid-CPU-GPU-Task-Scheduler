#ifndef METRICS_H
#define METRICS_H

#include <string>
#include <chrono>
#include <iostream>

class metrics {
public:
    // metrics(const std::string& taskName);

    // Start the timer
    void start();

    // Stop the timer and print elapsed time
    void stop();

    // Get elapsed milliseconds without printing
    double elapsedMs() const;

private:
    std::string taskName;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
};

#endif // METRICS_H
