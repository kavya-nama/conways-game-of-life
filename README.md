# Conway's Game of Life

Terminal-based Conway's Game of Life in C++.
Uses `+` for alive cells and ` ` for dead cells.
Displays a live header with grid size, refresh rate, and turn counter.

![demo](assets/demo.gif)

## Build

Requires `cmake` >= 3.10 and a C++17 compiler.

```bash
cmake -B build
cmake --build build
```

## Usage

```bash
./build/game_of_life [rows cols refresh_rate_ms turns]
```

| Argument        | Default value | Description                       |
|-----------------|---------------|-----------------------------------|
| rows            | 50            | Grid height                       |
| cols            | 100           | Grid width                        |
| refresh_rate_ms | 200           | Delay between generations (ms)    |
| turns           | 100           | Number of generations to simulate |

Examples:

```bash
./build/game_of_life --help
./build/game_of_life
./build/game_of_life 20 75 200 200
```

Grid is randomly initialized (50% alive) each run. Boundaries are dead (no wrapping).

