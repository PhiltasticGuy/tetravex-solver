#include "MultithreadedSolver.h"

Piece* MultithreadedSolver::solve(const int piece, bool &isComplete) {
    Piece* solution = new Piece[_size];
    bool states[_size];
    std::fill(states, states+_size, false);
    
    // Set the starting piece.
    std::vector<solve_state> stack;
    stack.push_back(solve_state{0, piece, 0});
    while(!stack.empty() && !isComplete) {
        // {
        //     std::lock_guard<std::mutex> lock(_mutex);
        //     if (isComplete) break;
        // }
        solve_state current = stack.back();

        // cout << "Position: " << current.position << endl;
        if (current.position == _size) {
            // std::lock_guard<std::mutex> lock(_mutex);
            isComplete = true;
            return solution;
        }
        else {
            bool hasPlacedPiece = false;
            for (int i = current.piece; i < _size; i++) {
                // cout << "\tPiece: " << i
                //     << " - " << _pieces[i] << endl;
                if (!states[i]) {
                    if (isValidMove(solution, &_pieces[i], current.position)) {
                        states[i] = true;
                        solution[current.position] = _pieces[i];

                        // PUSH!!!
                        stack.push_back(solve_state{current.position+1, 0, i});
                        hasPlacedPiece = true;
                        break;
                    }
                }
            }

            if (!hasPlacedPiece) {
                // cout << "\tNo valid pieces for position " << current.position << "!" << endl;
                
                // Undo the move since it was invalid!
                states[current.popPiece] = false;
                solution[current.position] = Piece::empty();
                
                stack.pop_back();

                // The piece following the one we pushed onto the stack.
                stack.back().piece = current.popPiece + 1;
            }
        }
    }

    if (solution != NULL) {
        delete[] solution;
    }

    return NULL;
}