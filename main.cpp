#include <iostream>
#include <SDL2/SDL.h>

const int TitleSize = 30; // 10 pixels
const int Rows = 20, Columns = 10;

const int WIDTH = Columns * TitleSize;
const int HEIGHT = Rows * TitleSize;

int deltatime = 0; // 1 second = 1000 milliseconds;
int current_time = SDL_GetTicks();
int last_time = current_time;

bool Field[Rows][Columns] = { 0 };

struct TetrominoData {
  int shape[4][4];
  int color_r, color_g, color_b;
};

TetrominoData Tetrominos[7] = {
  {
    {
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    255, 0, 0
  },
  {
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    0, 255, 0
  },
  {
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0}
    },
    0, 0, 255
  },
  {
    {
      {0, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    255, 255, 0
  },
  {
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0}
    },
    255, 0, 255
  },
  {
    {
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 0}
    },
    0, 255, 255
  },
  {
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0}
    },
    255, 165, 0
  }
};

class Tetromino {
  public:
  int x, y;
  TetrominoData data;
  Tetromino() {
    x = Columns / 2 - 2;
    y = -1;
    getData();
  }
  void getData() {
    data = Tetrominos[rand() % 7];
  }
  bool isCollided() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (data.shape[i][j]) {
          if (i + y >= Rows || j + x < 0 || j + x >= Columns || Field[i + y][j + x]) {
            return true;
          }
        }
      }
    }
    return false;
  }
  void rotate() {
    int temp[4][4];
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        temp[i][j] = data.shape[i][j];
      }
    }
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        data.shape[i][j] = temp[3 - j][i];
      }
    }
    if (isCollided()) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          data.shape[i][j] = temp[i][j];
        }
      }
    }
  }
  void hardDrop() {
    while (!isCollided()) {
      y++;
    }
    y--;
  }
};

Tetromino CurrentTetromino = Tetromino();

void destroyLine() {
  // Loop through each row, check for full row
  for (int i = Rows - 1; i >= 0; i--) {
    bool is_full = true;
    for (int j = 0; j < Columns; j++) {
      if (!Field[i][j]) {
        is_full = false;
        break;
      }
    }
    if (is_full) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < Columns; j++) {
          Field[k][j] = Field[k - 1][j];
        }
      }
      i++;
    }
  }
}

void update() {
  current_time = SDL_GetTicks();
  deltatime = current_time - last_time;
  if (deltatime > 500) {
    //Move down
    CurrentTetromino.y++;
    last_time = current_time;
    deltatime = 0;
    if (CurrentTetromino.isCollided()) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (CurrentTetromino.data.shape[i][j]) {
            Field[CurrentTetromino.y + i - 1][CurrentTetromino.x + j] = 1;
          }
        }
      }
      Tetromino* tetromino = new Tetromino();
      CurrentTetromino = *tetromino;
      destroyLine();
    }
  }
}

void render(SDL_Renderer* renderer) {
  //Draw field
  for (int i = 0; i < Rows; i++) {
    for (int j = 0; j < Columns; j++) {
      if (!Field[i][j]) {
        SDL_Rect tile = { j * TitleSize + 2, i * TitleSize + 2, TitleSize - 4, TitleSize - 4 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &tile);
      }
    }
  }
  //Draw tetronimo
  SDL_SetRenderDrawColor(renderer, CurrentTetromino.data.color_r, CurrentTetromino.data.color_g, CurrentTetromino.data.color_b, 255);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (CurrentTetromino.data.shape[i][j]) {
        SDL_Rect tile = { (CurrentTetromino.x + j) * TitleSize + 2, (CurrentTetromino.y + i) * TitleSize + 2, TitleSize - 4, TitleSize - 4 };
        SDL_RenderFillRect(renderer, &tile);
      }
    }
  }
  //Present
  SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("The Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

  if (NULL == window) {
    std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Event window_event;
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //Set background color
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  bool is_running = true;
  while (is_running) {
    if (SDL_PollEvent(&window_event)) {
      if (window_event.type == SDL_QUIT || window_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        is_running = false;
      }
      if (window_event.type == SDL_KEYDOWN) {
        switch (window_event.key.keysym.scancode) {
          case SDL_SCANCODE_A:
            CurrentTetromino.x--;
            if (CurrentTetromino.isCollided()) {
              CurrentTetromino.x++;
            }
            break;
          case SDL_SCANCODE_D:
            CurrentTetromino.x++;
            if (CurrentTetromino.isCollided()) {
              CurrentTetromino.x--;
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
    }
    update();
    render(renderer);
    if (Field[0][Columns / 2] && CurrentTetromino.isCollided()) {
      is_running = false;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}