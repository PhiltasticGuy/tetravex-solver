#pragma once

#include <vector>
#include "ITetravexSolver.h"

class SequentialSolver: public ITetravexSolver {
    public:
        SequentialSolver(int width, std::vector<Piece> pieces);
        ~SequentialSolver();
        bool solve(const int position);
};