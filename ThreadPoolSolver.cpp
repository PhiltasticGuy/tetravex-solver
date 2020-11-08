#include "ThreadPoolSolver.h"

ThreadPoolSolver::ThreadPoolSolver()
    : TetravexSolver{}
{
}

ThreadPoolSolver::~ThreadPoolSolver()
{
}

Piece* ThreadPoolSolver::solve(const std::vector<Piece> pieces) {
    Piece* solution;
    
    // Définir le nombre maximum de thread égal au nombre total de pièces. 
    ThreadPool tp(_size);
    bool isComplete = false;
    for (int piece = 0; piece < _size; piece++) {
        auto action = [&isComplete, this, &solution, piece] {
            auto temp = MultithreadedSolver::solveAction(piece, isComplete);
            if (temp != NULL) solution = temp;
        };

        tp.add(action);
    }
    tp.stop();

    return solution;
}