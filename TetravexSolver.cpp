#include "TetravexSolver.h"

TetravexSolver::TetravexSolver(const std::string filePath) {
    _pieces = loadGameData(filePath);
}

bool TetravexSolver::isFileExists(const std::string filename) const {
    return static_cast<bool>(std::ifstream(filename));
}

std::vector<Piece> TetravexSolver::loadGameData(std::string filePath) {
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

    _width = char2digit(line[0]);
    _height = char2digit(line[2]);
    _size = _width * _height;

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

    return pieces;
}

void TetravexSolver::debug() const {
    std::cout << "==================================================" << std::endl
         << " Width: " << _width << std::endl
         << "Height: " << _height << std::endl
         << "  Size: " << _size << std::endl
         << std::endl
         << "Available Pieces: " << std::endl;
    
    for (int i = 0; i < _pieces.size(); i++) {
        std::cout << _pieces[i] << std::endl;
    }

    std::cout << "==================================================" << std::endl
         << std::endl;
}