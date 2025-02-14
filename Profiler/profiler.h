#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>

// Profiling utility
class Profiler {
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        total_time += duration.count();
        count++;
    }

    double average_time() const {
        return (count == 0) ? 0.0 : static_cast<double>(total_time) / count;
    }

    void reset() {
        total_time = 0;
        count = 0;
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    int64_t total_time = 0;
    int64_t count = 0;
};

#endif // PROFILER_H
