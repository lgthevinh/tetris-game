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

- Use the A and D keys to move the tetromino left and right.
- Use the W key for hard drop, or the S key for soft drop.
- Use the space key to rotate the tetromino.

# Source code structure

### Constants variables
There are some constants variables that you can change to customize the game:

```cpp
  const int TileSize = 30; // This variable hold the size of each cell in the grid (pixel)

  const Rows = 20, Columns = 10; // These variables hold the number of rows and columns in the grid (for my game, it is 20x10)

  const set_times = 700; // This variable hold the time interval between each drop of the tetromino (milisecond)

  const int WIDTH = Columns * TileSize + 200; // 200 pixels for scoring display
  const int HEIGHT = Rows * TileSize; // These variables is automatically calculated based on the number of rows and columns
  ```


  ### Data and objects structure:

  ```cpp
  // The cell structure hold the data of each cell in the grid, including the color of the cell and whether the cell is filled or not
  struct Cell {
    bool isFilled = false;
    int color_r = 255, color_g = 255, color_b = 255;
  };

  // The TetrominoData structure includes the shape of the tetromino stored in a 4x4 matrix, and the color of the tetromino
  struct TetrominoData {
    int shape[4][4];
    int color_r, color_g, color_b;
  };

  // An array storing 7 different types of tetrominoes
  TetrominoData Tetrominos[7] = {
    {
      {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
      },
      255, 0, 0 // This is the shape of the I tetromino (example)
    },
    ...
  };

  Cell Field[Rows][Columns]; // The grid of the game

  class Tetromino {
    public:
    int x, y; // The position of the tetromino
    int ghost_x, ghost_y; // The position of the ghost tetromino
    TetrominoData data; // The data of the tetromino
    
    Tetromino(); // Constructor
    void getData(); // This function is used to get the data of the tetromino randomly
    bool isCollided(); // This function is used to check if the tetromino is collided with the grid or other tetrominoes
    void rotate();
    void hardDrop();
    void getGhost(); // This function is used to get the position of the ghost tetromino
}
```
### Game logic

The game has two main functions: update and render. The update function is used to update the game state, while the render function is used to render the game.

```cpp
  void render() { // This function is used to render the game
    /* 
      The tetromino is rendered by drawing cells in a for loop that iterates through the 4x4 matrix of the tetromino data. The same is done for the ghost tetromino.
    */
    SDL_SetRenderDrawColor(renderer, CurrentTetromino.data.color_r, CurrentTetromino.data.color_g, CurrentTetromino.data.color_b, 255);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (CurrentTetromino.data.shape[i][j]) {
          SDL_Rect tile = { (CurrentTetromino.x + j) * TileSize + 2, (CurrentTetromino.y + i) * TileSize + 2, TileSize - 4, TileSize - 4 };
          SDL_RenderFillRect(renderer, &tile);
        }
      }
    }



  };  
  
  void update() { // This function is used to update the game state
    destroyLine(); // This function is used to destroy the line if it is full, the logic behind is to check the grid rows if it is full, then destroy it and move the above rows down (for more detail, please check the source code)

    if (deltatime > 500 - (score * 10)) {
      // This logic hold the time interval between each drop of the tetromino, the interval will be decreased based on the score (decreased by 10 milisecond for each score)
      //Move down
      CurrentTetromino.y++;
      last_time = current_time;
      deltatime = 0;
    } 
    
    // This logic is used to check if the tetromino is collided with the grid or other tetrominoes
    // If the tetromino is collided, then the tetromino will be placed on the grid (update cells data based on tetromino data) and a new tetromino will be generated
    if (CurrentTetromino.isCollided()) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (CurrentTetromino.data.shape[i][j]) {
            Field[CurrentTetromino.y + i - 1][CurrentTetromino.x + j].isFilled = true;
            Field[CurrentTetromino.y + i - 1][CurrentTetromino.x + j].color_r = CurrentTetromino.data.color_r;
            Field[CurrentTetromino.y + i - 1][CurrentTetromino.x + j].color_g = CurrentTetromino.data.color_g;
            Field[CurrentTetromino.y + i - 1][CurrentTetromino.x + j].color_b = CurrentTetromino.data.color_b;
          }
        }
      }

      Tetromino* tetromino = new Tetromino();
      CurrentTetromino = NextTetromino;
      NextTetromino = *tetromino;

      delete tetromino;
    }
  }; 
```

### Game control flow

The game control flow is implemented using the SDL2 library. The game loop is implemented using the following code:

```cpp
  bool is_running = true;
  while (is_running) {
    if (SDL_PollEvent(&window_event)) {

      if (window_event.type == SDL_QUIT || window_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        is_running = false;
      }

      if (window_event.type == SDL_KEYDOWN && !CurrentTetromino.isCollided()) {
        // This is the control flow of the game, the tetromino can be moved left, right, down, rotated, or hard dropped
        switch (window_event.key.keysym.scancode) {
          case SDL_SCANCODE_A:
            CurrentTetromino.x--;
            CurrentTetromino.ghost_x--;
            if (CurrentTetromino.isCollided()) { // Check if the tetromino is collided with the grid or other tetrominoes, then move back
              CurrentTetromino.x++;
              CurrentTetromino.ghost_x++;
            }
            break;
          case SDL_SCANCODE_D:
            CurrentTetromino.x++;
            CurrentTetromino.ghost_x++;
            if (CurrentTetromino.isCollided()) {
              CurrentTetromino.x--;
              CurrentTetromino.ghost_x--;
            }
            break;
          case SDL_SCANCODE_S:
            CurrentTetromino.y++;
            if (CurrentTetromino.isCollided()) {
              CurrentTetromino.y--;
            }
            break;
          case SDL_SCANCODE_W:
            CurrentTetromino.hardDrop();
            break;
          case SDL_SCANCODE_SPACE:
            CurrentTetromino.rotate();
            break;
          default:
            break;
        }
      }
      
      // If the first row of the grid is filled and the current tetromino is collided, then the game is over
      if (Field[0][Columns / 2].isFilled && CurrentTetromino.isCollided()) {
        is_running = false;
      }
    }

    update();
    render();
  }
```
