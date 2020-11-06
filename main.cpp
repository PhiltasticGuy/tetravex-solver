#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "Board.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "ERREUR: Vous devez specifier un fichier de donnees pour la partie.";
        return -1;
    }

    Board b(argv[1]);
    b.debug();

    // b.solveSequential(0);
    // displaySolution(b.getSize(), b.getWidth(), b.getSolution());
    b.solve();

    return 0;
}