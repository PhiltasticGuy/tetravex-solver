#include "Board.h"

Board::Board(const std::string filePath) {
    _pieces = loadGameData(filePath);
    _solution = new Piece[_size];
    
    // _solver = new SequentialSolver(1, std::vector<Piece>());
}

Board::~Board() {
    if (_solution != NULL) {
        delete[] _solution;
    }
    // if (_solver != NULL) {
    //     delete _solver;
    // }
}

bool Board::isFileExists(const std::string filename) const {
    return static_cast<bool>(std::ifstream(filename));
}

std::vector<Piece> Board::loadGameData(std::string filePath) {
    if (!isFileExists(filePath)) {
        cout << "ERREUR: Le fichier specife n'a pas pu etre charge.";
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

int Board::getSize() const {
    return _size;
}

int Board::getWidth() const {
    return _width;
}

Piece* Board::getSolution() {
    // _tp.stop();
    return _solution;
}

void Board::debug() const {
    cout << "==================================================" << endl
         << " Width: " << _width << endl
         << "Height: " << _height << endl
         << "  Size: " << _size << endl
         << endl
         << "Available Pieces: " << endl;
    
    for (int i = 0; i < _pieces.size(); i++) {
        cout << _pieces[i] << endl;
    }

    cout << "==================================================" << endl
         << endl;
}

// Piece* Board::solve() {
//     this->_solver->solve(0);
//     return this->_solver->getSolution();
// }

bool Board::solve() {
    Piece* solution;
    auto begin = std::chrono::steady_clock::now();
    if (solveRecursive(0)) solution = _solution;
    // Piece* solution = solveIteration();
    // solution = solveThreads();
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("Temps d'execution: %.8fs\n", s);
    cout << endl;
    this->displaySolution(solution);

    if (solution != NULL) {
        delete[] solution;
    }

    return false;
}

bool Board::solveRecursive(const int position) {
    // cout << "Position: " << position << endl;
    if (position == _size) {
        return true;
    }
    else {
        for (int i = 0; i < _size; i++) {
            // cout << "\tPiece: " << i
            //      << " - " << _pieces[i] << endl;
            if (!_pieces[i].isUsed()) {
                if (isValidMove(_solution, &_pieces[i], position)) {
                    _pieces[i].setUsed(true);
                    _solution[position] = _pieces[i];

                    if (solveRecursive(position+1)) {
                        return true;
                    }
                    
                    // Undo the move since it was invalid!
                    // cout << "\tNo valid pieces for position " << position+1 << "!" << endl;
                    _solution[position] = Piece::empty();
                    _pieces[i].setUsed(false);
                    // cout << "Position: " << position << endl;
                }
            }
        }
    }

    return false;
}

Piece* Board::solveIteration() {
    Piece* solution = new Piece[_size];
    
    bool states[_size];
    std::fill(states, states+_size, false);
    
    // Set the starting piece.
    std::vector<solve_state> stack = {solve_state{0, 0, 0}};
    
    bool isComplete = false;
    while(!stack.empty() && !isComplete) {
        solve_state current = stack.back();

        cout << "Position: " << current.position << endl;
        if (current.position == _size) {
            isComplete = true;
        }
        else {
            bool hasPlacedPiece = false;
            for (int i = current.piece; i < _size; i++) {
                cout << "\tPiece: " << i
                     << " - " << _pieces[i] << endl;
                if (!states[i]) {
                    if (isValidMove(solution, &_pieces[i], current.position)) {
                        states[i] = true;
                        solution[current.position] = _pieces[i];

                        // PUSH!!!
                        stack.push_back(solve_state{current.position+1, 0, i});
                        hasPlacedPiece = true;
                        break;
                    }
                }
            }

            if (!hasPlacedPiece) {
                cout << "\tNo valid pieces for position " << current.position << "!" << endl;
                
                // Undo the move since it was invalid!
                states[current.popPiece] = false;
                solution[current.position] = Piece::empty();
                
                stack.pop_back();

                // The piece following the one we pushed onto the stack.
                stack.back().piece = current.popPiece + 1;
            }
        }
    }

    cout << "Is Completed: " << (isComplete ? "true" : "false") << endl;

    return solution;
}

Piece* Board::solveThreads() {
    Piece* result;
    std::vector<std::thread> vt;

    bool isComplete = false;
    for (int piece = 0; piece < _size; piece++) {
        auto action = [&isComplete, this, &result, piece]() {
            Piece* solution = new Piece[_size];
            bool states[_size];
            std::fill(states, states+_size, false);
            
            // Set the starting piece.
            std::vector<solve_state> stack;
            stack.push_back(solve_state{0, piece, 0});

            while(!stack.empty() && !isComplete) {
                solve_state current = stack.back();

                // cout << "Position: " << current.position << endl;
                if (current.position == _size) {
                    isComplete = true; // MUTEX ME!!!
                    result = solution; // MUTEX ME!!!
                    return;
                }
                else {
                    bool hasPlacedPiece = false;
                    for (int i = current.piece; i < _size; i++) {
                        // cout << "\tPiece: " << i
                        //     << " - " << _pieces[i] << endl;
                        if (!states[i]) {
                            if (isValidMove(solution, &_pieces[i], current.position)) {
                                states[i] = true;
                                solution[current.position] = _pieces[i];

                                // PUSH!!!
                                stack.push_back(solve_state{current.position+1, 0, i});
                                hasPlacedPiece = true;
                                break;
                            }
                        }
                    }

                    if (!hasPlacedPiece) {
                        // cout << "\tNo valid pieces for position " << current.position << "!" << endl;
                        
                        // Undo the move since it was invalid!
                        states[current.popPiece] = false;
                        solution[current.position] = Piece::empty();
                        
                        stack.pop_back();

                        // The piece following the one we pushed onto the stack.
                        stack.back().piece = current.popPiece + 1;
                    }
                }
            }

            if (solution != NULL) {
                delete[] solution;
            }
        };

        vt.push_back(std::thread(action));
    }

    for (auto &t : vt) {
        t.join();
    }

    return result;
}

bool Board::isValidMove(const Piece* solution, const Piece* piece, const int index) const {
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

void Board::displaySolution(Piece* solution) const {
    std::cout << "==================================================" << std::endl
         << "Solution: " << std::endl;

    for (int i = 0; i < _size; i++) {
        std::cout << solution[i] << " ";
        
        if ((i + 1) % _width == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << "==================================================" << std::endl;
}
