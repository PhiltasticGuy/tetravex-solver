#pragma once

#include <iostream>
#include <chrono>
#include "Piece.h"

int char2digit(char c);
void displaySolution(const int size, const int width, Piece* solution);

template<typename F>
double stopwatch(F func) {
    auto begin = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    return elapsed.count() / 1e+9;
}
