#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

// : char tells the enum what storage primitive to use, char = 1 byte
enum class CellState : char { Alive = 1, Dead = 0 };
enum class CellStateSymbol : char { Alive = '+', Dead = ' ' };

// overloading << on cout since CellStateSymbol is a scoped enum which has no
// implicit convsion to char
std::ostream &operator<<(std::ostream &os, CellStateSymbol s) {
    return os << static_cast<char>(s);
}

// alias
using Grid = std::vector<std::vector<CellState>>;

class Game {
  private:
    int rows;
    int cols;
    Grid grid, nextGrid;

  public:
    Game(int x, int y, double cell_alive_probability = 0.5)
        : rows(x), cols(y), grid(x, std::vector<CellState>(y, CellState::Dead)),
          nextGrid(x, std::vector<CellState>(y, CellState::Dead)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution coin_flip(cell_alive_probability);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] =
                    coin_flip(gen) ? CellState::Alive : CellState::Dead;
            }
        }
    }
    void print() {
        for (auto row : grid) {
            for (CellState cell : row) {
                std::cout << (cell == CellState::Alive ? CellStateSymbol::Alive
                                                       : CellStateSymbol::Dead);
            }
            // we dont use std::endl here since it always flushes output, we
            // want to flush the output once every frame and not once every row
            std::cout << "\n";
        }
        std::cout << std::flush;
    }

    int count_neighbors(int i, int j) {
        int neighbors = 0;
        // 1 2 3
        // 4 X 5
        // 6 7 8
        if (i == 0) {
            // first row, neighbors 1 2 3 are permanently Dead
            if (j == 0) {
                // first column, neighbors 1 4 6 are permanently Dead
                if (grid[i + 1][j] == CellState::Alive)
                    neighbors++; // 7
                if (grid[i][j + 1] == CellState::Alive)
                    neighbors++; // 5
                if (grid[i + 1][j + 1] == CellState::Alive)
                    neighbors++; // 8
            } else if (j == cols - 1) {
                // last column, neighbors 3 5 8 are permanently Dead
                if (grid[i][j - 1] == CellState::Alive)
                    neighbors++; // 4
                if (grid[i + 1][j - 1] == CellState::Alive)
                    neighbors++; // 6
                if (grid[i + 1][j] == CellState::Alive)
                    neighbors++; // 7
            } else {
                // any middle column
                if (grid[i][j - 1] == CellState::Alive)
                    neighbors++; // 4
                if (grid[i][j + 1] == CellState::Alive)
                    neighbors++; // 5
                if (grid[i + 1][j - 1] == CellState::Alive)
                    neighbors++; // 6
                if (grid[i + 1][j] == CellState::Alive)
                    neighbors++; // 7
                if (grid[i + 1][j + 1] == CellState::Alive)
                    neighbors++; // 8
            }
        } else if (i == rows - 1) {
            // last row, neighbors 6 7 8 are permanently Dead
            if (j == 0) {
                // first column, neighbors 1 4 6 are permanently Dead
                if (grid[i - 1][j] == CellState::Alive)
                    neighbors++; // 2
                if (grid[i - 1][j + 1] == CellState::Alive)
                    neighbors++; // 3
                if (grid[i][j + 1] == CellState::Alive)
                    neighbors++; // 5

            } else if (j == cols - 1) {
                // last column, neighbors 3 5 8 are permanently Dead
                if (grid[i - 1][j - 1] == CellState::Alive)
                    neighbors++; // 1
                if (grid[i - 1][j] == CellState::Alive)
                    neighbors++; // 2
                if (grid[i][j - 1] == CellState::Alive)
                    neighbors++; // 4
            } else {
                // any middle column
                if (grid[i - 1][j - 1] == CellState::Alive)
                    neighbors++; // 1
                if (grid[i - 1][j] == CellState::Alive)
                    neighbors++; // 2
                if (grid[i - 1][j + 1] == CellState::Alive)
                    neighbors++; // 3
                if (grid[i][j - 1] == CellState::Alive)
                    neighbors++; // 4
                if (grid[i][j + 1] == CellState::Alive)
                    neighbors++; // 5
            }
        } else {
            // any middle row
            if (j == 0) {
                // first column, neighbors 1 4 6 are permanently Dead
                if (grid[i - 1][j] == CellState::Alive)
                    neighbors++; // 2
                if (grid[i - 1][j + 1] == CellState::Alive)
                    neighbors++; // 3
                if (grid[i][j + 1] == CellState::Alive)
                    neighbors++; // 5
                if (grid[i + 1][j] == CellState::Alive)
                    neighbors++; // 7
                if (grid[i + 1][j + 1] == CellState::Alive)
                    neighbors++; // 8
            } else if (j == cols - 1) {
                // last column, neighbors 3 5 8 are permanently Dead
                if (grid[i - 1][j - 1] == CellState::Alive)
                    neighbors++; // 1
                if (grid[i - 1][j] == CellState::Alive)
                    neighbors++; // 2
                if (grid[i][j - 1] == CellState::Alive)
                    neighbors++; // 4
                if (grid[i + 1][j - 1] == CellState::Alive)
                    neighbors++; // 6
                if (grid[i + 1][j] == CellState::Alive)
                    neighbors++; // 7
            } else {
                // any middle column
                if (grid[i - 1][j - 1] == CellState::Alive)
                    neighbors++; // 1
                if (grid[i - 1][j] == CellState::Alive)
                    neighbors++; // 2
                if (grid[i - 1][j + 1] == CellState::Alive)
                    neighbors++; // 3
                if (grid[i][j - 1] == CellState::Alive)
                    neighbors++; // 4
                if (grid[i][j + 1] == CellState::Alive)
                    neighbors++; // 5
                if (grid[i + 1][j - 1] == CellState::Alive)
                    neighbors++; // 6
                if (grid[i + 1][j] == CellState::Alive)
                    neighbors++; // 7
                if (grid[i + 1][j + 1] == CellState::Alive)
                    neighbors++; // 8
            }
        }
        return neighbors;
    }

    /*
     * Rules for next generatoin:
     *
     * Dead + 3 neighbors = Alive (Birth)
     * Dead + n neighbors (n != 3) = Dead (No change)
     * Alive + 0 or 1 neighbors = Dead (Underpopulation)
     * Alive + 2 or 3 neighbors = Alive (Survival)
     * Alive + 4 or more neighbors = Dead (Overpopulation)
     * */
    void step() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int neighbors = count_neighbors(i, j);
                if (grid[i][j] == CellState::Alive) {
                    // currently Alive
                    if (neighbors == 0 || neighbors == 1) {
                        // underpopulation
                        nextGrid[i][j] = CellState::Dead;
                    } else if (neighbors == 2 || neighbors == 3) {
                        // survival
                        nextGrid[i][j] = CellState::Alive;
                    } else {
                        // overpopulation
                        nextGrid[i][j] = CellState::Dead;
                    }
                } else {
                    // currently Dead
                    if (neighbors == 3) {
                        // birth
                        nextGrid[i][j] = CellState::Alive;
                    } else {
                        // no change
                        nextGrid[i][j] = CellState::Dead;
                    }
                }
            }
        }
        // swapping the grid and nextGrid, since all cells in nextGrid will be
        // overwritten anyway it doesn't matter that it has the old game board
        // values
        std::swap(grid, nextGrid);
    }
};

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void reset_cursor_up(int lines) {
    // \033[ = ANSI control sequence prefix
    // A     = Cursor Up command
    std::cout << "\033[" << lines << "A\r";
}

void print_game_info(int rows, int cols, int refresh_rate_ms, int turns,
                     int turns_elapsed) {
    std::cout << "Grid: " << rows << "x" << cols
              << "\tRefresh Rate : " << refresh_rate_ms
              << "ms\tTurn: " << turns_elapsed << "/" << turns << "\n";
}

int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 5) {
        std::cout << "Usage: " << argv[0]
                  << " [rows cols turns refresh_rate_ms]\n";
        return 1;
    }

    int rows = 50, cols = 100, turns = 100, refresh_rate_ms = 200;
    if (argc == 5) {
        rows = std::stoi(argv[1]);
        cols = std::stoi(argv[2]);
        turns = std::stoi(argv[3]);
        refresh_rate_ms = std::stoi(argv[4]);
    }

    Game game(rows, cols);
    int turns_remaining = turns;
    print_game_info(rows, cols, refresh_rate_ms, turns,
                    turns - turns_remaining);
    game.print();
    while (turns_remaining--) {
        sleep(refresh_rate_ms);
        game.step();
        reset_cursor_up(rows + 1);
        print_game_info(rows, cols, refresh_rate_ms, turns,
                        turns - turns_remaining);

        game.print();
    };
    return 0;
}
