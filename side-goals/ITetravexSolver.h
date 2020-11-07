#pragma once

#include <vector>
#include "Piece.h"

class ITetravexSolver {
protected:
    int _width;
    int _size;
    std::vector<Piece> _pieces;
    Piece* _solution;

public:
    ITetravexSolver(int width, std::vector<Piece> pieces) {
        _width = width;
        _size = width * width;
        _pieces = pieces;
        _solution = new Piece[_size];
    };
    virtual ~ITetravexSolver() {
        if (_solution != NULL) {
            delete _solution;
        }
    };

    virtual bool solve(const int position);

    Piece* getSolution() {
        return _solution;
    }

    bool isValidMove(const Piece* piece, const int index) const {
        const int leftIdx = index - 1,
            topIdx = index - _width,
            rightIdx = index + 1,
            bottomIdx = index + _width;

        // cout << "==================================================" << endl
        //     << " Piece: " << *piece << endl
        //     << " Index: " << index << endl
        //     << "  Left: " << leftIdx << " " << (index % _width != 0 ? _solution[leftIdx] : Piece()) << endl
        //     << "   Top: " << topIdx << " " << (index >= _width ? _solution[topIdx] : Piece()) << endl
        //     << " Right: " << rightIdx << " " << ((index + 1) % _width != 0 ? _solution[rightIdx] : Piece()) << endl
        //     << "Bottom: " << bottomIdx << " " << (index + _width < _size ? _solution[bottomIdx] : Piece()) << endl
        //     // << piece << " at position " << index << " is " << (isValid ? "Valid" : "Invalid") << endl
        //     << "==================================================" << endl;

        if ((index % _width == 0 || _solution[leftIdx].isValidAtLeftOf(piece)) &&
            (index < _width  || _solution[topIdx].isValidAtTopOf(piece)) &&
            (rightIdx % _width == 0 || _solution[rightIdx].isValidAtRightOf(piece)) &&
            (bottomIdx >= _size || _solution[bottomIdx].isValidAtBottomOf(piece))) 
        {
            return true;
        }

        return false;
    }
};