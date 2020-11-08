#include <iostream>
#include "TetravexSolver.h"
#include "IterativeSolver.h"
#include "ThreadVectorSolver.h"
#include "ThreadPoolSolver.h"

struct board {
    std::vector<Piece> pieces;
    const int width;
};

bool isFileExists(const std::string filename) {
    return static_cast<bool>(std::ifstream(filename));
}

board loadGameData(std::string filePath) {
    if (!isFileExists(filePath)) {
        std::cout << "ERREUR: Le fichier specife n'a pas pu etre charge.";
        return {};
    }

    std::ifstream file(filePath);
    std::string line;

    // Lire et traiter la première ligne contenant la taille du Tetravex. 
    std::getline(file, line);
    if (line.length() != 3) {
        throw std::invalid_argument("ERREUR: La premiere ligne du fichier doit obligatoirement contenir 3 caracteres.");
    }

    int width = char2digit(line[0]);
    int height = char2digit(line[2]);
    int size = width * height;

    // Lire et instancier toutes les pièces du Tetravex.
    std::vector<Piece> pieces;
    while (std::getline(file, line)) {
        if (line.length() != 7) {
            throw std::invalid_argument("ERREUR: Une ligne representant une piece tetravex doit obligatoirement contenir 7 caracteres.");
        }

        pieces.push_back(
            Piece(char2digit(line[0]), char2digit(line[2]), char2digit(line[4]), char2digit(line[6]))
        );
    }

    file.close();

    return board{ pieces = pieces, width = width };
}

void debug(board board) {
    std::cout << "==================================================" << std::endl
         << " Width: " << board.width << std::endl
         << "  Size: " << board.pieces.size() << std::endl
         << std::endl
         << "Available Pieces: " << std::endl;
    
    for (int i = 0; i < board.pieces.size(); i++) {
        std::cout << board.pieces[i] << std::endl;
    }

    std::cout << "==================================================" << std::endl
         << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "ERREUR: Vous devez specifier un fichier de donnees pour la partie." << std::endl;
        return -1;
    }

    board b = loadGameData(argv[1]);
    debug(b);

    // TetravexSolver *solver = new IterativeSolver(b.pieces, b.width);
    // TetravexSolver *solver = new ThreadVectorSolver(b.pieces, b.width);
    TetravexSolver *solver = new ThreadPoolSolver(b.pieces, b.width);
    solver->solve();

    if (solver != NULL) {
        delete solver;
    }

    return 0;
}