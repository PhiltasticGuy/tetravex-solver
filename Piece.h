#pragma once

#include <iostream>

class Piece {
private:
    static const int INDEX_LEFT = 0;
    static const int INDEX_TOP = 1;
    static const int INDEX_RIGHT = 2;
    static const int INDEX_BOTTOM = 3;

    int _values[4] = { -1, -1, -1, -1 };

public:
    static Piece& empty() { 
        static Piece _emptyInstance;
        return _emptyInstance;
    }

    Piece();
    Piece(const int leftValue, const int topValue, const int rightValue, const int bottomValue);

    friend bool operator== (const Piece &lhs, const Piece &rhs);

    int GetLeft() const;
    int GetTop() const;
    int GetRight() const;
    int GetBottom() const;

    bool isEmptyPiece() const;

    bool isValidAtLeftOf(const Piece* piece) const;
    bool isValidAtTopOf(const Piece* piece) const;
    bool isValidAtRightOf(const Piece* piece) const;
    bool isValidAtBottomOf(const Piece* piece) const;
};

std::ostream& operator<<(std::ostream& stream, const Piece& piece);