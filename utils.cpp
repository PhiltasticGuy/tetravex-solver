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