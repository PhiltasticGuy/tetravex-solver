#include "RecursiveSolver.h"

RecursiveSolver::RecursiveSolver()
    : TetravexSolver{}
{
}

RecursiveSolver::~RecursiveSolver()
{
    if (_states != NULL) {
        delete[] _states;
    }
}

bool RecursiveSolver::solveRecursive(const int position) {
    // cout << "Position: " << position << endl;
    if (position == _size) {
        return true;
    }
    else {
        for (int i = 0; i < _size; i++) {
            // cout << "\tPiece: " << i
            //      << " - " << _pieces[i] << endl;
            if (!_states[i]) {
                if (isValidMove(_solution, &_pieces[i], position)) {
                    _states[i] = true;
                    _solution[position] = _pieces[i];

                    if (solveRecursive(position+1)) {
                        return true;
                    }
                    
                    // Undo the move since it was invalid!
                    // cout << "\tNo valid pieces for position " << position+1 << "!" << endl;
                    _solution[position] = Piece::empty();
                    _states[i] = false;
                    // cout << "Position: " << position << endl;
                }
            }
        }
    }

    return false;
}

Piece* RecursiveSolver::solve(const std::vector<Piece> pieces) {
    _solution = new Piece[pieces.size()];
    _states = new bool[pieces.size()];
    std::fill(_states, _states + pieces.size(), false);
    
    solveRecursive(0);

    return _solution;
}