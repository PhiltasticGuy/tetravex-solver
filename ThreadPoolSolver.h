#pragma once

#include <thread>
#include <mutex>
#include "MultithreadedSolver.h"
#include "ThreadPool.h"

class ThreadPoolSolver: virtual public MultithreadedSolver {
private:
    std::mutex _mutex;

public:
    ThreadPoolSolver(const std::string filePath);
    ~ThreadPoolSolver();
    void solve();
};