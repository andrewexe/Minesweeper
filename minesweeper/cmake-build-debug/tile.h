#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Tile {
private:
    int xpos;
    int ypos;
    bool hasFlag;
    bool hasMine;
    bool revealed;
    int surroundingMines;
    vector<Tile*> adjacentTiles;

public:
    Tile() {
        xpos = 0;
        ypos = 0;
        hasFlag = false;
        hasMine = false;
        revealed = false;
        surroundingMines = 0;
    }

    Tile(int& x, int& y, bool hasflag, bool hasmine, bool revealed) {
        xpos = x;
        ypos = y;
        this->hasFlag = hasflag;
        this->hasMine = hasmine;
        this->revealed = revealed;
        surroundingMines = 0;
    }

    bool gethasflag() const {
        return hasFlag;
    }

    bool gethasmine() const {
        return hasMine;
    }

    bool getrevealed() const {
        return revealed;
    }

    int getsurroundingmines() const {
        return surroundingMines;
    }

    void toggleFlag() {
        hasFlag = !hasFlag;
    }

    void setasmine() {
        hasMine = true;
    }

    void reveal() {
        revealed = true;
    }

    int getypos() const {
        return ypos;
    }

    int getxpos() const {
        return xpos;
    }

    void setneighbors(vector<vector<Tile>>& tilegrid, int rows, int columns) {
        int positiony[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        int positionx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
        for (int i = 0; i < 8; i++) {
            int row = ypos + positiony[i];
            int column = xpos + positionx[i];
            if (row >= 0 && column >= 0 && row < rows && column < columns) {
                adjacentTiles.push_back(&tilegrid[row][column]);
            }
        }
    }

    void countsurroundingmines() {
        for (const auto& neighbor : adjacentTiles) {
            if (neighbor->gethasmine()) {
                surroundingMines++;
            }
        }
    }

    void revealneighbors(vector<vector<Tile>>& tilegrid, int rows, int columns) {
        if (!revealed && !hasMine) {
            revealed = true;

            if (surroundingMines == 0) {
                for (const auto& neighbor : adjacentTiles) {
                    neighbor->revealneighbors(tilegrid, rows, columns);
                }
            }
        }
    }
};
