#pragma once

#include <thread>
#include <mutex>
#include "MultithreadedSolver.h"

class ThreadVectorSolver: virtual public MultithreadedSolver {
private:
    std::mutex _mutex;

    Piece* solve(const std::vector<Piece> pieces);
    
public:
    ThreadVectorSolver();
    ~ThreadVectorSolver();
    void solve();
};