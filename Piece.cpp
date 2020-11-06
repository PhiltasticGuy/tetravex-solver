#include "Piece.h"

Piece::Piece() {
}

Piece::Piece(const int leftValue, const int topValue, const int rightValue, const int bottomValue) : Piece() 
{
    _values[0] = leftValue;
    _values[1] = topValue;
    _values[2] = rightValue;
    _values[3] = bottomValue;
}

int Piece::GetLeft() const {
    return _values[Piece::INDEX_LEFT];
}

int Piece::GetTop() const {
    return _values[Piece::INDEX_TOP];
}

int Piece::GetRight() const {
    return _values[Piece::INDEX_RIGHT];
}

int Piece::GetBottom() const {
    return _values[Piece::INDEX_BOTTOM];
}

bool Piece::isUsed() const {
    return _isUsed;
}

void Piece::setUsed(const bool value) {
    _isUsed = value;
}

std::ostream& operator<<(std::ostream& stream, const Piece& piece) {
    return stream << "[" << piece.GetLeft() << " " << piece.GetTop() << " " 
                    << piece.GetRight() << " " << piece.GetBottom() << "]";
}

bool Piece::isEmptyPiece() const {
    return this == &empty();
}

bool Piece::isValidAtLeftOf(const Piece* piece) const {
    return this->_values[Piece::INDEX_RIGHT] == -1 || this->_values[Piece::INDEX_RIGHT] == piece->GetLeft()/* || this->isEmptyPiece()*/;
}

bool Piece::isValidAtTopOf(const Piece* piece) const {
    return this->_values[Piece::INDEX_BOTTOM] == -1 || this->_values[Piece::INDEX_BOTTOM] == piece->GetTop()/* || this->isEmptyPiece()*/;
}

bool Piece::isValidAtRightOf(const Piece* piece) const {
    return this->_values[Piece::INDEX_LEFT] == -1 || this->_values[Piece::INDEX_LEFT] == piece->GetRight()/* || this->isEmptyPiece()*/;
}

bool Piece::isValidAtBottomOf(const Piece* piece) const {
    // std::cout << *piece << std::endl
    //           << *this << std::endl
    //           << this->_values[Piece::INDEX_TOP] << " == " << piece->GetBottom();
    return this->_values[Piece::INDEX_TOP] == -1 || this->_values[Piece::INDEX_TOP] == piece->GetBottom()/* || this->isEmptyPiece()*/;
}