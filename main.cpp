#include <iostream>
#include "TetravexSolver.h"
#include "IterativeSolver.h"
#include "ThreadVectorSolver.h"
#include "ThreadPoolSolver.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    // if (argc < 2) {
    //     cout << "ERREUR: Vous devez specifier un fichier de donnees pour la partie." << endl;
    //     return -1;
    // }

    // bool isBenchmarkTest = false;
    // std::string filename;
    // for (int i = 1; i < argc; i++) {
    //     if (argv[1] == "-b" || argv[1] == "--benchmark") {
    //         isBenchmarkTest = true;
    //     }
    //     else if (argv[1] == "-f" || argv[1] == "--file") {
    //         filename = argv[i];
    //     }
    // }

    std::string filenames[] = {
        "./data/2x2.txt",
        "./data/3x3.txt",
        "./data/4x4.txt",
        "./data/5x5.txt",
        "./data/6x6.txt",
        "./data/7x7.txt"/*,
        "./data/8x8.txt",*/
    };

    TetravexSolver *solver;

    //######################################################################//
    // Sequential - Iterative
    //######################################################################//
    cout << "======================================================================" << endl
         << " SEQUENTIAL - ITERATIVE" << endl
         << "======================================================================" << endl;
    solver = new IterativeSolver();
    for(std::string filename : filenames) {
        cout << "------------------------------------------------------------" << endl
             << "Filename: " << filename << endl
             << endl;
        solver->solve(filename);
        cout << "------------------------------------------------------------" << endl;
    }
    if (solver != NULL) {
        delete solver;
    }
    //######################################################################//

    //######################################################################//
    // Multithreaded - Thread Vector
    //######################################################################//
    cout << "======================================================================" << endl
         << " MULTITHREADED - THREAD VECTOR" << endl
         << "======================================================================" << endl;
    solver = new ThreadVectorSolver();
    // solver->solve("./data/6x6.txt");
    for(std::string filename : filenames) {
        cout << "------------------------------------------------------------" << endl
             << "Filename: " << filename << endl
             << endl;
        solver->solve(filename);
        cout << "------------------------------------------------------------" << endl;
    }
    if (solver != NULL) {
        delete solver;
    }
    //######################################################################//

    //######################################################################//
    // MULTITHREADED - THREAD POOL
    //######################################################################//
    cout << "======================================================================" << endl
         << " MULTITHREADED - THREAD POOL" << endl
         << "======================================================================" << endl;
    solver = new ThreadPoolSolver();
    for(std::string filename : filenames) {
        cout << "------------------------------------------------------------" << endl
             << "Filename: " << filename << endl
             << endl;
        solver->solve(filename);
        cout << "------------------------------------------------------------" << endl;
    }
    if (solver != NULL) {
        delete solver;
    }
    //######################################################################//

    return 0;
}