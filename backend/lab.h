#pragma once

#include <cstdlib>
#include <fstream>
#include <ratio>
#include <sstream>
#include <string>
#include <vector>
#include "vars.h"

enum class WeightType {
    MINUS_INF,
    PLUS_INF,
    NUM, 
    UNDEF,
    ZERO
};

class Weight {
    WeightType m_wt = WeightType::UNDEF;
    int m_wval = 0;

public:
    Weight() {}

    Weight(WeightType wt, int wval = 0) : m_wt(wt) {
        if (wt == WeightType::NUM)
            m_wval = wval;
    } 
};

class Robot;
class Field;

class LabCell {

friend Field;
friend Robot;

    int cell_num;

    LabCell* adj[6];
    

    CellType status;
    Weight weight;

    LabCell(CellType ct, int w = 0) : status(ct){
        switch (ct) {
            case CellType::BOX:
                weight = Weight(WeightType::NUM, w);
                break;
            case CellType::WALL:
                weight = Weight(WeightType::MINUS_INF);
                break;
            case CellType::EMPTY:
                weight = Weight(WeightType::ZERO);
                break;
            case CellType::EXIT:
                weight = Weight(WeightType::PLUS_INF);
            default:
                weight = Weight(WeightType::UNDEF);
        }
    }

    bool is_barrier () {
        switch (status) {
            case CellType::BOX:
            case CellType::WALL:
                return true;
            default:
                return false;
        }
    }
};

class Field {
    

public:

std::vector<LabCell> cells;

    Field (const std::string &string) {
        std::ifstream fs(string);
        std::string str;
        int num = 0;
        while (fs.getline(str.data(), 100)) {
            std::stringstream strstream(str);
            std::string token;
            std::getline(strstream, token, ' ');

            switch (token[1]) {
                case 'B':
                    cells[num] = LabCell(CellType::BOX, token[2]-'0');
                    break;
                case 'Q':
                    cells[num] = LabCell(CellType::EXIT);
                    break;
                case 'E':
                    cells[num] = LabCell(CellType::EMPTY);
                    break;
                case 'W':
                    cells[num] = LabCell(CellType::WALL);
                    break;
            }

            for (int i = 0; i < 6; i++) {
                std::getline(strstream, token, ' ');
                cells[num].adj[i] = &cells[i];
            } 

            num++;
        }
    }
};

enum class Orient {
    NORTH, 
    NORTH_EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    NORTH_WEST
};

class Robot {
    LabCell &cur_cell;
    Orient direction;

public:

    Robot (Field &field) : cur_cell(field.cells[0]) {}

    void print () {
        std::cout << cur_cell.cell_num << std::endl;
        std::cout << (int) direction << std::endl;
    }

    Boolean forward () {
        cur_cell = *cur_cell.adj[(int) direction];
        return true;
    }

    Boolean back() { 
        cur_cell = *cur_cell.adj[std::abs(((int) direction) - 3)];
        return true;
    }

    Boolean right () {
        int new_dir = (int) direction; 
        if (direction != Orient::NORTH_WEST)
            direction = (Orient) ++new_dir;
        else 
            direction = Orient::NORTH;

        return true;
    }

    Boolean left() { 
        int new_dir = (int) direction;
        if (direction != Orient::NORTH) 
            direction = (Orient) --new_dir;
        else 
            direction = Orient::NORTH_WEST;

        return true;
    }

    Integer look() {

        LabCell next_cell = cur_cell;
        int dist = 0;

        while (!next_cell.is_barrier()) {
            next_cell = *cur_cell.adj[(int) direction];
            ++dist;
        }

        return dist;
    }

    Cell test() {
        LabCell next_cell = cur_cell;

        while (!next_cell.is_barrier()) {
            next_cell = *cur_cell.adj[(int) direction];
        }

        return next_cell.status;
    }
};