#include "ThreadPoolSolver.h"

ThreadPoolSolver::ThreadPoolSolver(const std::string filePath)
    : TetravexSolver{filePath}
{
}

ThreadPoolSolver::~ThreadPoolSolver()
{
}

void ThreadPoolSolver::solve() {
    auto begin = std::chrono::steady_clock::now();
    Piece* result;
    
    // Définir le nombre maximum de thread égal au nombre total de pièces. 
    ThreadPool tp(_size);
    bool isComplete = false;
    for (int piece = 0; piece < _size; piece++) {
        auto action = [&isComplete, this, &result, piece] {
            auto temp = MultithreadedSolver::solve(piece, isComplete);
            if (temp != NULL) result = temp;
        };

        tp.add(action);
    }
    tp.stop();
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("\nTemps d'execution: %.8fs\n\n", s);
    
    TetravexSolver::displaySolution(result);

    if (result != NULL) {
        delete[] result;
    };
}