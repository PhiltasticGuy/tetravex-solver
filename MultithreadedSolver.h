#pragma once

#include "TetravexSolver.h"

class MultithreadedSolver: virtual public TetravexSolver {
public:
    Piece* solve(const int piece, bool &isComplete);
    virtual void solve() =0;
};