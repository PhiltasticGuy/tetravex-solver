#include "IterativeSolver.h"

IterativeSolver::IterativeSolver(const std::string filePath)
    : TetravexSolver{filePath}
{
}

IterativeSolver::~IterativeSolver()
{
}

void IterativeSolver::solve() {
    auto begin = std::chrono::steady_clock::now();
    Piece* solution = new Piece[_size];
    
    bool states[_size];
    std::fill(states, states+_size, false);
    
    // Set the starting piece.
    std::vector<solve_state> stack = {solve_state{0, 0, 0}};
    
    bool isComplete = false;
    while(!stack.empty() && !isComplete) {
        solve_state current = stack.back();

        // cout << "Position: " << current.position << endl;
        if (current.position == _size) {
            isComplete = true;
        }
        else {
            bool hasPlacedPiece = false;
            for (int i = current.piece; i < _size; i++) {
                // cout << "\tPiece: " << i
                //      << " - " << _pieces[i] << endl;
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
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("Temps d'execution: %.8fs\n\n", s);

    TetravexSolver::displaySolution(solution);

    if (solution != NULL) {
        delete[] solution;
    };
}