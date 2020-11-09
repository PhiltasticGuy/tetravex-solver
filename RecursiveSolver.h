#pragma once

#include "TetravexSolver.h"

using std::cout;
using std::endl;

class RecursiveSolver: virtual public TetravexSolver {
private:
    Piece* _solution;
    bool* _states;

    bool solveRecursive(const int position);
    Piece* solve(const std::vector<Piece> pieces);

public:
    RecursiveSolver();
    ~RecursiveSolver();
};