#pragma once

#include <thread>
#include <mutex>
#include "MultithreadedSolver.h"
#include "ThreadPool.h"

class ThreadPoolSolver: virtual public MultithreadedSolver {
private:
    std::mutex _mutex;

    Piece* solve(const std::vector<Piece> pieces);

public:
    ThreadPoolSolver();
    ~ThreadPoolSolver();
};