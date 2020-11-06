#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
#include "Piece.h"
#include "ThreadPool.h"
#include <chrono>
#include <omp.h>
#include "utils.h"
// #include "ITetravexSolver.h"
// #include "SequentialSolver.h"

using std::cout;
using std::endl;

struct solve_state {
    int position = -1;
    int piece = -1;
    int popPiece = -1;
    int direction = -1;
    bool attempts[4] = { false, false, false, false };
};

class Board {
private:
    static const int DIRECTION_NONE = -1;
    static const int DIRECTION_RIGHT = 0;
    static const int DIRECTION_BOTTOM = 1;
    static const int DIRECTION_LEFT = 2;
    static const int DIRECTION_TOP = 3;

    int _size;
    int _width;
    int _height;

    std::vector<Piece> _pieces;
    Piece* _solution;

    // SequentialSolver* _solver;
    // ThreadPool _tp = ThreadPool(1);

    bool isFileExists(const std::string filename) const;
    std::vector<Piece> loadGameData(const std::string filePath);

    int* getAdjacentPositions(const int index) const;
    bool isValidMove(const Piece* solution, const Piece* piece, const int index) const;
    void displaySolution(Piece* solution);
    void displaySolution(std::vector<Piece> solution);
public:
    Board();
    Board(const std::string filePath);
    ~Board();

    int getSize() const;
    int getWidth() const;
    Piece* getSolution();
    
    void debug() const;

    bool solve();
    Piece* solveStack();
    Piece* solveStack2();
    bool solveSequential(int position);
    Piece* solveTetra();
    bool solveOpenMP(int position);
    bool solve3(int position);
};