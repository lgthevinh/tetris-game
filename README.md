The Tetris Game
========

This is a repository for the Tetris game. This project is a part of the course "Advanced Programming" at the University of Engineering and Technology, Vietnam National University, Hanoi.

# Introduction

In this project, I implemented the Tetris game using the C++ programming language. The game will be implemented using the SDL2 library.

# Table of contents

- [Introduction](#introduction)
- [Table of contents](#table-of-contents)
- [Game features](#game-features)

# Game features 

Basically this game inherits the orginal Tetris game, but all of the feature were developed without inheriting open-source. The game has the following features:

- The game has a grid with a size of 10x20 cells.
- The game has 7 different types of tetrominoes.
- The tetrominoes can be rotated.
- The tetrominoes can be moved left, right, down.
- The tetrominoes can be dropped instantly or dropped gradually.
- Ghost tetrominoes are displayed on the grid.
- Next tetrominoes preview
- The game has a score system.

# Installation and execution

To install the game, you need to have the SDL2 library installed on your computer. You can install the SDL2 library by following the instructions on the [SDL2 website](https://www.libsdl.org/download-2.0.php).

After installing the SDL2 library, you can clone this repository to your computer using the following command:

```bash
git clone [repository_url]
```

To run the game, you need to compile the source code using the following command:

```bash
g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o main
```

or if you have the Makefile, you can run the following command:

```bash
make
```


# Game instructions

- Use the left and right arrow keys to move the tetromino left and right.
- Use the up arrow key for hard drop, or the down arrow key for soft drop.
- Use the space key to rotate the tetromino.

# Source code structure

There are some constains variables that you can change to customize the game:

```cpp
const int TileSize = 30; // This variable hold the size of each cell in the grid (pixel)

const Rows = 20, Columns = 10; // These variables hold the number of rows and columns in the grid (for my game, it is 20x10)

const set_times = 700; // This variable hold the time interval between each drop of the tetromino (milisecond)

const int WIDTH = Columns * TileSize + 200; // 200 pixels for scoring display
const int HEIGHT = Rows * TileSize; // These variables is automatically calculated based on the number of rows and columns
```
