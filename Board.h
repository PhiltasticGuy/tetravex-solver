#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <omp.h>
#include "utils.h"
#include "Piece.h"
#include "ThreadPool.h"

using std::cout;
using std::endl;

struct solve_state {
    int position = -1;
    int piece = -1;
    int popPiece = -1;
};

class Board {
private:
    int _size;
    int _width;
    int _height;

    std::vector<Piece> _pieces;
    std::mutex _mutex;

    bool isFileExists(const std::string filename) const;
    std::vector<Piece> loadGameData(const std::string filePath);

    int* getAdjacentPositions(const int index) const;
    bool isValidMove(const Piece* solution, const Piece* piece, const int index) const;
    void displaySolution(Piece* solution)const;

    Piece* solve(const int piece, bool &isComplete);
public:
    Board();
    Board(const std::string filePath);
    ~Board();

    int getSize() const;
    int getWidth() const;
    Piece* getSolution();
    
    void debug() const;

    bool solve();
    // bool solveRecursive(int position);
    Piece* solveIteration();
    Piece* solveThreads();
    Piece* solveThreadPool();
    bool solveOpenMP(int position);
};