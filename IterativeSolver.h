#pragma once

#include "TetravexSolver.h"

class IterativeSolver: virtual public TetravexSolver {
private:
    Piece* solve(const std::vector<Piece> pieces);

public:
    IterativeSolver(const std::vector<Piece> pieces, const int width);
    ~IterativeSolver();
};