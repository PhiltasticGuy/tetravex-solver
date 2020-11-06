#include "utils.h"

int char2digit(char c) {
    if (c < '0' || c > '9') {
        throw std::invalid_argument("ERREUR: Le caractere ne figure pas entre '0' et '9'.");
    }

    // On transforme le numéro de type "char" à sa value numérique.
    // '0' - '0' => 0
    // '1' - '0' => 1
    // ...
    // '9' - '0' => 9
    return c - '0';
}

void displaySolution(const int size, const int width, Piece* solution) {
    std::cout << "==================================================" << std::endl
         << "Solution: " << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << solution[i] << " ";
        
        if ((i + 1) % width == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << "==================================================" << std::endl;
}