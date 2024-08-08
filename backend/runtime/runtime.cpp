#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include "runtime.hpp"


Boolean Field::move_robot(bool forward) {
    prev_cell = cur_cell;
    bool status = move_from_cell(forward, cur_cell);
    if (!status) std::cout << "stuck" << std::endl;
    if (cur_cell == exit) throw std::runtime_error("YOU WON");
    return status;
}

Field::Field (const std::string &string) {
    
    std::ifstream fs(string);
    if (!fs.is_open()) {
        throw std::runtime_error("Error: Unable to open the file.\n");
    }

    std::string token_str;
    int cur_cell_idx = 0;
    int depth = 0;
    int width = 1;
    m_width = width;

    while (std::getline(fs, token_str)) {
        std::istringstream strstream(token_str);

        std::string cell_str;

        while (std::getline(strstream, cell_str, ' ')) {

            CellType cur_cell_type;
            int weight = 0;

            switch (cell_str[0]) {
                case 'R': 
                    cur_cell = Cell(depth, width);
                    prev_cell = cur_cell;
                    break;
                case 'B':
                    cur_cell_type = CellType::BOX;
                    weight = atoi(cell_str.c_str()+1);
                    break;
                case 'Q':
                    cur_cell_type = CellType::EXIT;
                    exit = Cell(depth, width);
                    break;
                case 'E':
                    cur_cell_type = CellType::EMPTY;
                    break;
                case 'W':
                    cur_cell_type = CellType::WALL;
                    break;
            }

            m_cells.push_back(std::pair(Cell(depth, width), BaseCell(cur_cell_type, weight)));
            width+=2;
            cur_cell_idx++;
        }
        depth++;

        m_width = (m_width >= width) ? m_width : width;

        width = (width%2) ? 0 : 1;
    }

    m_depth = depth;
    m_width--;

    fs.close();

    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/sock.sock");
    
    int connection_result = connect(m_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connection_result == -1) {
        perror("Socket does not connected: ");
        close(m_socket);
        m_socket = -1;
    }
}


Boolean Field::right() {
    int new_dir = (int) direction;
    if (direction != Orient::NORTH_WEST)
        direction = (Orient) ++new_dir;
    else
        direction = Orient::NORTH;

    return true;
}

Boolean Field::left() {
    int new_dir = (int) direction;
    if (direction != Orient::NORTH)
        direction = (Orient) --new_dir;
    else
        direction = Orient::NORTH_WEST;

    return true;
}

bool change_coords (const Field &field, Cell &cell_to_move_from, int d, int w) {
    int new_d = cell_to_move_from.first + d;
    if (new_d > field.get_depth() || new_d < 0)
        return false;
    int new_w = cell_to_move_from.second + w;
    if (new_d > field.get_width() || new_w < 0)
        return false;

    cell_to_move_from = Cell(new_d, new_w);
    return true;
}

bool move_n(const Field &field, Cell &start_cell) {
    return change_coords(field, start_cell, -2, 0);
}

bool move_s(const Field &field, Cell &start_cell) {
    return change_coords(field, start_cell, 2, 0);
}

bool move_nw(const Field &field, Cell &start_cell) {
    return change_coords(field, start_cell, -1, -1);
}

bool move_ne(const Field &field, Cell &start_cell) {
    return change_coords(field, start_cell, -1, 1);
}

bool move_se(const Field &field, Cell &start_cell) {
    return change_coords(field, start_cell, 1, 1);
}

bool move_sw(const Field &field, Cell &start_cell) {
    return change_coords(field, start_cell, 1, -1);
}


bool Field::move_from_cell(bool forward, Cell &start_cell) {

    switch (direction) {
        case Orient::NORTH:
            return forward ? move_n(*this, start_cell) 
                            : move_s(*this, start_cell);
        case Orient::NORTH_EAST:
            return forward ? move_ne(*this, start_cell) 
                            : move_sw(*this, start_cell);
        case Orient::SOUTH_EAST:
            return forward ? move_se(*this, start_cell) 
                            : move_nw(*this, start_cell);
        case Orient::SOUTH:
            return forward ? move_s(*this, start_cell) 
                            : move_n(*this, start_cell);
        case Orient::SOUTH_WEST:
            return forward ? move_sw(*this, start_cell) 
                            : move_ne(*this, start_cell);
        case Orient::NORTH_WEST:
            return forward ? move_nw(*this, start_cell) 
                            : move_se(*this, start_cell);
    }

    return false;
}

const CellType get_cell_type(const Cell &to_find, const Field &field) {
    for (const auto &entry : field.get_cells())
        if (entry.first.first == to_find.first && entry.first.second == to_find.second)
            return entry.second.m_type;

    return CellType::UNDEF;    
}

bool is_barrier (const Cell &to_look, const Field &field) {
    switch 
    (get_cell_type(to_look, field)) {
        case CellType::BOX:
        case CellType::WALL:
            return true;
        default:
            return false;
    }
}


CellType Field::test() {
    Cell to_test = cur_cell;
    while (move_from_cell(1, to_test))
        if (is_barrier(to_test, *this)) 
            return 
            get_cell_type(to_test, *this);

    return CellType::WALL;
}

Integer Field::look() {
    Cell to_look = cur_cell;
    int dist = 0;
    while (move_from_cell(1, to_look)) {
        dist++;
        if (is_barrier(to_look, *this)) return dist;
    }

    return dist;
}

void 
Field::send() {
    std::cout << "New pos {" << cur_cell.first << ", " << cur_cell.second <<"}, dir=" << (uint32_t)direction << "\n";
    State state{cur_cell, direction};
    if (m_socket != -1) {
        write(m_socket, &state, sizeof(state));
    }
    sleep(1);
}