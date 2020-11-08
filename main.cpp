#include <iostream>
#include "TetravexSolver.h"
#include "IterativeSolver.h"
#include "ThreadVectorSolver.h"
#include "ThreadPoolSolver.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "ERREUR: Vous devez specifier un fichier de donnees pour la partie." << std::endl;
        return -1;
    }

    // TetravexSolver *solver = new IterativeSolver(argv[1]);
    // TetravexSolver *solver = new ThreadVectorSolver(argv[1]);
    TetravexSolver *solver = new ThreadPoolSolver(argv[1]);
    solver->solve();

    if (solver != NULL) {
        delete solver;
    }

    return 0;
}