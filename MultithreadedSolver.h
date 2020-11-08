#pragma once

#include "TetravexSolver.h"

class MultithreadedSolver: virtual public TetravexSolver {
private:
    virtual Piece* solve(const std::vector<Piece> pieces) =0;
    
protected:
    Piece* solveAction(const int piece, bool &isComplete);
};