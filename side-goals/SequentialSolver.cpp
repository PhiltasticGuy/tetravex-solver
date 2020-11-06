#include "SequentialSolver.h"

SequentialSolver::SequentialSolver(int width, std::vector<Piece> pieces)
    : ITetravexSolver( width, pieces )
{
}

SequentialSolver::~SequentialSolver()
{
}

bool SequentialSolver::solve(const int position) {
    // cout << "Position: " << position << endl;
    if (position == _size) {
        return true;
    }
    else {
        for (int i = 0; i < _size; i++) {
            // cout << "\tPiece: " << i
            //      << " - " << _pieces[i] << endl;
            //     //  << " - " << (!_pieces[i].isUsed() && isValidMove(&_pieces[i], position) ? "Processed" : "  Skipped") << endl;
            if (!_pieces[i].isUsed() && isValidMove(&_pieces[i], position)) {
                _pieces[i].setUsed(true);
                _solution[position] = _pieces[i];
                
                if (solve(position+1)) {
                    return true;
                }
                
                // // Undo the move since it was invalid!
                // cout << "\tNo valid pieces for position " << position << "!" << endl
                //      << "Position: " << position << endl;
                _solution[position] = Piece::empty();
                _pieces[i].setUsed(false);
            }
        }
    }

    return false;
}