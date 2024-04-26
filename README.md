# The Tetris Game

This is a repository for the Tetris game. This project is a part of the course "Advanced Programming" at the University of Engieering and Technology, Vietnam National University, Hanoi.

## Introduction

In this project, we will implement the Tetris game using the C++ programming language. The game will be implemented using the SDL2 library.

## Features

- The game has main features of the original Tetris game.
- The game has a simple and intuitive interface.
- The game has a scoring system.

## Installation

To install the game, you need to have the SDL2 library installed on your computer. You can install the SDL2 library by following the instructions on the [SDL2 website](https://www.libsdl.org/download-2.0.php).

After installing the SDL2 library, you can clone this repository to your computer using the following command:

```bash
git clone [repository_url]
```

## Usage

To run the game, you need to compile the source code using the following command:

```bash
g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o main
```
or
```bash
make
```
for the Makefile

## Game instructions

- Use the left and right arrow keys to move the tetromino left and right.
- Use the up arrow key for hard drop, or the down arrow key for soft drop.
- Use the space key to rotate the tetromino.