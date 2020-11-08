#include "IterativeSolver.h"

IterativeSolver::IterativeSolver(const std::vector<Piece> pieces, const int width)
    : TetravexSolver{pieces, width}
{
}

IterativeSolver::~IterativeSolver()
{
}

Piece* IterativeSolver::solve(const std::vector<Piece> pieces) {
    Piece* solution = new Piece[pieces.size()];
    
    bool states[pieces.size()];
    std::fill(states, states+pieces.size(), false);
    
    // Set the starting piece.
    std::vector<solve_state> stack = { solve_state{0, 0, 0} };
    
    bool isComplete = false;
    while(!stack.empty() && !isComplete) {
        solve_state current = stack.back();

        // cout << "Position: " << current.position << endl;
        if (current.position == pieces.size()) {
            isComplete = true;
        }
        else {
            bool hasPlacedPiece = false;
            for (int i = current.piece; i < pieces.size(); i++) {
                // cout << "\tPiece: " << i
                //      << " - " << pieces[i] << endl;
                if (!states[i]) {
                    if (isValidMove(solution, &pieces[i], current.position)) {
                        states[i] = true;
                        solution[current.position] = pieces[i];

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

    return solution;
}