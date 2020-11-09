#include <iostream>
#include "TetravexSolver.h"
#include "RecursiveSolver.h"
#include "IterativeSolver.h"
#include "ThreadVectorSolver.h"
#include "ThreadPoolSolver.h"

using std::cout;
using std::endl;

const int SOLVER_RECURSIVE = 0;
const int SOLVER_ITERATIVE = 1;
const int SOLVER_THREAD_VECTOR = 2;
const int SOLVER_THREAD_POOL = 3;

const std::string FILENAMES[] = {
    "./data/2x2.txt",
    "./data/3x3.txt",
    "./data/4x4.txt",
    "./data/5x5.txt",
    "./data/6x6.txt",
    "./data/7x7.txt",
    // "./data/8x8.txt",
};

void runSolver(const int solveType) {
    TetravexSolver *solver = NULL;
    std::string title;

    switch (solveType)
    {
    case SOLVER_RECURSIVE:
        solver = new RecursiveSolver();
        title = "SEQUENTIAL - RECURSIVE";
        break;
    case SOLVER_ITERATIVE:
        solver = new IterativeSolver();
        title = "SEQUENTIAL - ITERATIVE";
        break;
    case SOLVER_THREAD_VECTOR:
        solver = new ThreadVectorSolver();
        title = "MULTITHREADED - THREAD VECTOR";
        break;
    case SOLVER_THREAD_POOL:
        solver = new ThreadPoolSolver();
        title = "MULTITHREADED - THREAD POOL";
        break;
    
    default:
        throw std::invalid_argument("ERREUR: Le type de solver choisi n'existe pas...");
    }

    cout << "======================================================================" << endl
         << " "<< title << endl
         << "======================================================================" << endl;
    for(std::string filename : FILENAMES) {
        cout << "------------------------------------------------------------" << endl
             << "Filename: " << filename << endl
             << endl;
        solver->solve(filename);
        cout << "------------------------------------------------------------" << endl;
    }
    if (solver != NULL) {
        delete solver;
    }
}

int main(int argc, char** argv) {
    runSolver(SOLVER_RECURSIVE);
    runSolver(SOLVER_ITERATIVE);
    runSolver(SOLVER_THREAD_VECTOR);
    runSolver(SOLVER_THREAD_POOL);

    return 0;
}