#pragma once

#include <vector>
#include <fstream>
#include <iomanip>
#include "Piece.h"
#include "Utils.h"

class TetravexSolver {
private:
    bool isFileExists(const std::string filename) const;
    std::vector<Piece> loadGameData(const std::string filePath);

protected:
    struct solve_state {
        int position = -1;
        int piece = -1;
        int popPiece = -1;
    };
    
    int _width;
    int _height;
    int _size;
    std::vector<Piece> _pieces;

    bool isValidMove(const Piece* solution, const Piece* piece, const int index) const {
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

    void displaySolution(Piece* solution) const {
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

public:
    TetravexSolver(const std::string filePath);
    virtual ~TetravexSolver() {
    };

    virtual void solve() = 0;

    void debug() const;
};