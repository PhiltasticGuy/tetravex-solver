#include "TetravexSolver.h"

TetravexSolver::TetravexSolver(const std::vector<Piece> pieces, const int width) {
    _pieces = pieces;
    _width = width;
    _size = width * width;
}

bool TetravexSolver::isValidMove(const Piece* solution, const Piece* piece, const int index) const {
    const int leftIdx = index - 1,
        topIdx = index - _width,
        rightIdx = index + 1,
        bottomIdx = index + _width;

    if ((index % _width == 0 || solution[leftIdx].isValidAtLeftOf(piece)) &&
        (index < _width  || solution[topIdx].isValidAtTopOf(piece)) &&
        (rightIdx % _width == 0 || solution[rightIdx].isValidAtRightOf(piece)) &&
        (bottomIdx >= _size || solution[bottomIdx].isValidAtBottomOf(piece))) 
    {
        return true;
    }

    return false;
}

void TetravexSolver::displaySolution(Piece* solution) const {
    std::cout << "==================================================" << std::endl
        << "Solution: " << std::endl;

    for (int i = 0; i < _size; i++) {
        std::cout << solution[i] << " ";
        
        if ((i + 1) % _width == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << "==================================================" << std::endl;
}

void TetravexSolver::solve() {
    auto begin = std::chrono::steady_clock::now();
    Piece* solution = solve(_pieces);
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("Temps d'execution: %.8fs\n\n", s);

    TetravexSolver::displaySolution(solution);

    if (solution != NULL) {
        delete[] solution;
    };
}