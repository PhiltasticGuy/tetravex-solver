#pragma once

#include <vector>
#include <fstream>
#include <iomanip>
#include "Piece.h"
#include "Utils.h"

class TetravexSolver {
private:

    virtual Piece* solve(const std::vector<Piece> pieces) = 0;

protected:
    struct solve_state {
        int position = -1;
        int piece = -1;
        int popPiece = -1;
    };

    std::vector<Piece> _pieces;
    int _width;
    int _size;

    bool isValidMove(const Piece* solution, const Piece* piece, const int index) const;
    void displaySolution(Piece* solution) const;

public:
    TetravexSolver(const std::vector<Piece> pieces, const int width);
    virtual ~TetravexSolver() {
    };

    void solve();
};