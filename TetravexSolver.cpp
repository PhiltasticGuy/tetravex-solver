#include "TetravexSolver.h"

TetravexSolver::TetravexSolver() {
}

bool TetravexSolver::isValidMove(const Piece* solution, const Piece* piece, const int index) const {
    const int leftIdx = index - 1,
        topIdx = index - _width,
        rightIdx = index + 1,
        bottomIdx = index + _width;

    if ((index % _width == 0 || solution[leftIdx].isValidAtLeftOf(piece)) &&
        (index < _width  || solution[topIdx].isValidAtTopOf(piece)) &&
        (rightIdx % _width == 0 || solution[rightIdx].isValidAtRightOf(piece)) &&
        (bottomIdx >= _size || solution[bottomIdx].isValidAtBottomOf(piece))) 
    {
        return true;
    }

    return false;
}

void TetravexSolver::displaySolution(Piece* solution) const {
    std::cout << "Solution: " << std::endl;

    for (int i = 0; i < _size; i++) {
        std::cout << solution[i] << " ";
        
        if ((i + 1) % _width == 0) {
            std::cout << std::endl;
        }
    }
}

bool TetravexSolver::isFileExists(const std::string filename) {
    return static_cast<bool>(std::ifstream(filename));
}

std::vector<Piece> TetravexSolver::loadGameData(const std::string filePath) {
    if (!isFileExists(filePath)) {
        std::cout << "ERREUR: Le fichier [" << filePath << "] n'a pas pu etre charge." << std::endl;
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
    int height = char2digit(line[2]);
    _size = _width * height;

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

void TetravexSolver::debugPieces() {
    std::cout << "==================================================" << std::endl
         << " Width: " << _width << std::endl
         << "  Size: " << _pieces.size() << std::endl
         << std::endl
         << "Available Pieces: " << std::endl;
    
    for (Piece piece : _pieces) {
        std::cout << piece << std::endl;
    }

    std::cout << "==================================================" << std::endl
         << std::endl;
}

void TetravexSolver::solve(const std::string filename) {
    _pieces = loadGameData(filename);

    auto begin = std::chrono::steady_clock::now();
    Piece* solution = solve(_pieces);
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("Temps d'execution: %.8fs\n\n", s);

    TetravexSolver::displaySolution(solution);

    if (solution != NULL) {
        delete[] solution;
    };
}