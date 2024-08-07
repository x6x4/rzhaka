#pragma once

#include <string>
#include <vector>
#include "../vars.h"


struct BaseCell {
    CellType m_type;
    WeightType m_weight;
    int m_wval;

    BaseCell(CellType type, int wval = 0) : m_type(type) {
        switch (m_type) {
            case CellType::BOX:
                m_weight = WeightType::NUM;
                m_wval = wval;
                break;
            case CellType::WALL:
                m_weight = WeightType::MINUS_INF;
                break;
            case CellType::EMPTY:
                m_weight = WeightType::ZERO;
                break;
            case CellType::EXIT:
                m_weight = WeightType::PLUS_INF;
                break;
            default:
                m_weight = WeightType::UNDEF;
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

typedef std::pair <int, int> Cell;

class Field {

    Orient direction = Orient::NORTH;
    Cell cur_cell;
    Cell prev_cell;
    int m_width;
    int m_depth;

    std::vector<std::pair<Cell, BaseCell>> m_cells;

public:
    const auto get_cells() const {return m_cells;}
    const auto get_orient() const {return direction;}
    const auto get_prev() const {return prev_cell;}
    const auto get_cur() const {return cur_cell;}
    const auto get_depth() const {return m_depth;}
    const auto get_width() const {return m_width;}

    Field (const std::string &string);

    Boolean move_robot(bool forward);
    Boolean right();
    Boolean left();
    CellType test();
    Integer look();

private:
    bool move_from_cell(bool forward, Cell &start_cell);

};