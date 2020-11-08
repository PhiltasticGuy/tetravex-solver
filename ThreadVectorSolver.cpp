#include "ThreadVectorSolver.h"

ThreadVectorSolver::ThreadVectorSolver(const std::string filePath)
    : TetravexSolver{filePath}
{
}

ThreadVectorSolver::~ThreadVectorSolver()
{
}

void ThreadVectorSolver::solve() {
    auto begin = std::chrono::steady_clock::now();
    Piece* result;
    std::vector<std::thread> vt;

    bool isComplete = false;
    for (int piece = 0; piece < _size; piece++) {
        auto action = [&isComplete, this, &result, piece]() {
            auto begin = std::chrono::steady_clock::now();
            auto temp = MultithreadedSolver::solve(piece, isComplete);
            auto end = std::chrono::steady_clock::now();
            if (temp != NULL) result = temp;

            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            double s = elapsed.count() / 1e+9;

            {
                std::thread::id threadId = std::this_thread::get_id();
                std::lock_guard<std::mutex> lock(_mutex);
                printf("Thread 0x%x: END TASK (%.8fs)\n", threadId, s);
            }
        };

        vt.push_back(std::thread(action));
    }

    for (auto &t : vt) {
        t.join();
    }
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("\nTemps d'execution: %.8fs\n\n", s);

    TetravexSolver::displaySolution(result);

    if (result != NULL) {
        delete[] result;
    };
}