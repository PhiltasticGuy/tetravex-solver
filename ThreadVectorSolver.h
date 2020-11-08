#pragma once

#include <thread>
#include <mutex>
#include "MultithreadedSolver.h"

class ThreadVectorSolver: virtual public MultithreadedSolver {
private:
    std::mutex _mutex;

public:
    ThreadVectorSolver(const std::string filePath);
    ~ThreadVectorSolver();
    void solve();
};