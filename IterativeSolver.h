#pragma once

#include "TetravexSolver.h"

class IterativeSolver: virtual public TetravexSolver {
public:
    IterativeSolver(const std::string filePath);
    ~IterativeSolver();
    void solve();
};