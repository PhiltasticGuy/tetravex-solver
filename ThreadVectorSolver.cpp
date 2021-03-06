#include "ThreadVectorSolver.h"

ThreadVectorSolver::ThreadVectorSolver()
    : TetravexSolver{}
{
}

ThreadVectorSolver::~ThreadVectorSolver()
{
}

Piece* ThreadVectorSolver::solve(const std::vector<Piece> pieces) {
    Piece* solution;
    std::vector<std::thread> vt;

    bool isComplete = false;
    for (int piece = 0; piece < _size; piece++) {
        auto action = [&isComplete, this, &solution, piece]() {
            auto begin = std::chrono::steady_clock::now();
            auto temp = MultithreadedSolver::solveAction(piece, isComplete);
            auto end = std::chrono::steady_clock::now();
            if (temp != NULL) solution = temp;

            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            double s = elapsed.count() / 1e+9;

            {
                std::thread::id threadId = std::this_thread::get_id();
                std::lock_guard<std::mutex> lock(_mutex);
                std::cout << std::fixed << std::setprecision(8)
                    << "Thread: 0x" << std::hex << threadId << ": END TASK (" << s << "s)" << std::endl;
            }
        };

        vt.push_back(std::thread(action));
    }

    for (auto &t : vt) {
        t.join();
    }

    return solution;
}