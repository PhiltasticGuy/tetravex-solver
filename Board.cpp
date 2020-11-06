#include "Board.h"

Board::Board(const std::string filePath) {
    _pieces = loadGameData(filePath);
    _solution = new Piece[_size];
    
    // _solver = new SequentialSolver(1, std::vector<Piece>());
}

Board::~Board() {
    if (_solution != NULL) {
        delete _solution;
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
    auto begin = std::chrono::steady_clock::now();
    Piece* solution = solveIteration();
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double s = elapsed.count() / 1e+9;
    printf("Temps d'execution: %.8fs\n", s);
    cout << endl;
    this->displaySolution(solution);

    if (solution != NULL) {
        delete solution;
    }

    return false;
}

bool Board::solveRecursive(const int position) {
    cout << "Position: " << position << endl;
    if (position == _size) {
        return true;
    }
    else {
        for (int i = 0; i < _size; i++) {
            cout << "\tPiece: " << i
                 << " - " << _pieces[i] << endl;
                //  << " - " << (!_pieces[i].isUsed() && isValidMove(&_pieces[i], position) ? "Processed" : "  Skipped") << endl;
            if (!_pieces[i].isUsed()) {
                if (isValidMove(_solution, &_pieces[i], position)) {
                    _pieces[i].setUsed(true);
                    _solution[position] = _pieces[i];

                    if (solveRecursive(position+1)) {
                        return true;
                    }
                    
                    // Undo the move since it was invalid!
                    cout << "\tNo valid pieces for position " << position+1 << "!" << endl;
                    _solution[position] = Piece::empty();
                    _pieces[i].setUsed(false);
                    cout << "Position: " << position << endl;
                }
            }
        }
    }

    return false;
}

Piece* Board::solveIteration() {
    std::vector<solve_state> stack;
    Piece* solution = new Piece[_size];
    bool isComplete = false;
    
    bool states[_size];
    std::fill(states, states+_size, false);

    stack.push_back(solve_state{0, 0, 0, Board::DIRECTION_RIGHT});
    
    while(!stack.empty() && !isComplete) {
        solve_state current = stack.back();

        // cout << "Position: " << current.position << endl;
        if (current.position == _size) {
            isComplete = true;
        }
        else {
            bool hasPushed = false;
            for (int i = current.piece; i < _size; i++) {
                // cout << "\tPiece: " << i
                //      << " - " << _pieces[i] << endl;
                    //  << " - " << (!_pieces[i].isUsed() && isValidMove(&_pieces[i], position) ? "Processed" : "  Skipped") << endl;
                if (!states[i]) {
                    if (isValidMove(solution, &_pieces[i], current.position)) {
                        states[i] = true;
                        solution[current.position] = _pieces[i];

                        // PUSH!!!
                        stack.push_back(solve_state{current.position+1, 0, i, Board::DIRECTION_RIGHT});
                        hasPushed = true;
                        break;
                    }
                }
            }

            if (!hasPushed) {
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

    cout << "Is Completed: " << (isComplete ? "true" : "false") << endl;

    return solution;
}

bool Board::solveOpenMP(const int position) {
    if (position == _size) {
        return true;
    }

    bool isCompleted = false;
    std::vector<std::thread> vt;

    for (int i = 0; i < _size; i++) {
        auto action = [this, &i, position, &isCompleted] () {
            if (!_pieces[i].isUsed()) {
                const int leftIdx = position - 1,
                    topIdx = position - _width,
                    rightIdx = position + 1,
                    bottomIdx = position + _width;

                if ((position % _width == 0 || _solution[leftIdx].isValidAtLeftOf(&_pieces[i])) &&
                    (position < _width  || _solution[topIdx].isValidAtTopOf(&_pieces[i])) &&
                    (rightIdx % _width == 0 || _solution[rightIdx].isValidAtRightOf(&_pieces[i])) &&
                    (bottomIdx >= _size || _solution[bottomIdx].isValidAtBottomOf(&_pieces[i]))) 
                {
                    _pieces[i].setUsed(true);
                    _solution[position] = _pieces[i];

                    isCompleted = solveRecursive(position+1);
                    
                    if (isCompleted) {
                        i = _size;
                    }
                    else {
                        // Undo the move since it was invalid!
                        _solution[position] = Piece::empty();
                        _pieces[i].setUsed(false);
                    }
                }
            }
        };
        vt.push_back(std::thread(action));
    }

    for(int i = 0; i < vt.size(); i++) {
        vt[i].join();
    }

    return isCompleted;
}

bool Board::isValidMove(const Piece* solution, const Piece* piece, const int index) const {
    const int leftIdx = index - 1,
        topIdx = index - _width,
        rightIdx = index + 1,
        bottomIdx = index + _width;

// cout << "==================================================" << endl
//     << " Piece: " << *piece << endl
//     << " Index: " << index << endl
//     << "  Left: " << leftIdx << " " << (index % _width != 0 ? _solution[leftIdx] : Piece()) << endl
//     << "   Top: " << topIdx << " " << (index >= _width ? _solution[topIdx] : Piece()) << endl
//     << " Right: " << rightIdx << " " << ((index + 1) % _width != 0 ? _solution[rightIdx] : Piece()) << endl
//     << "Bottom: " << bottomIdx << " " << (index + _width < _size ? _solution[bottomIdx] : Piece()) << endl
//     // << piece << " at position " << index << " is " << (isValid ? "Valid" : "Invalid") << endl
//     << "==================================================" << endl;

    if ((index % _width == 0 || solution[leftIdx].isValidAtLeftOf(piece)) &&
        (index < _width  || solution[topIdx].isValidAtTopOf(piece)) &&
        (rightIdx % _width == 0 || solution[rightIdx].isValidAtRightOf(piece)) &&
        (bottomIdx >= _size || solution[bottomIdx].isValidAtBottomOf(piece))) 
    {
        return true;
    }

    return false;
}

void Board::displaySolution(Piece* solution) {
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

void Board::displaySolution(std::vector<Piece> solution) {
    std::cout << "==================================================" << std::endl
         << "Solution: " << std::endl;

    for (int i = 0; i < _size; i++) {
        if (i < solution.size()) {
            std::cout << solution[i] << " ";
        }
        else {  
            std::cout << Piece() << " ";
        }
        
        if ((i + 1) % _width == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << "==================================================" << std::endl;
}
