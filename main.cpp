#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int TitleSize = 30; // 10 pixels
const int Rows = 20, Columns = 10;

const int WIDTH = Columns * TitleSize + 200; // 200 pixels for scoring display
const int HEIGHT = Rows * TitleSize;

const int set_time = 700;

const int x_display = WIDTH - 150;
const int y_display = 200;

int deltatime = 0; // 1 second = 1000 milliseconds
int current_time = SDL_GetTicks();
int last_time = current_time;

int score = 0;

struct Cell {
  bool isFilled = false;
  int color_r = 255, color_g = 255, color_b = 255;
};

struct TetrominoData {
  int shape[4][4];
  int color_r, color_g, color_b;
};

Cell Field[Rows][Columns];

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
      {0, 0, 1, 0},
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
  int ghost_x, ghost_y;
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
          if (i + y >= Rows || j + x < 0 || j + x >= Columns || Field[i + y][j + x].isFilled) {
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
    // Move if tetromino is out of bounds
    if (x < 0) {
      x = 0;
    }
    if (x > Columns - 4) {
      x = Columns - 4;
    }
    
  }
  void hardDrop() {
    while (!isCollided()) {
      this->y++;
    }
    this->y--;
  }
  void getGhost() {
    ghost_x = x;
    int temp = y;
    while (!isCollided()) {
      y++;
    }
    y--;
    ghost_y = y;
    y = temp;
  }
};

void destroyLine() {
  // Loop through each row, check for full row
  for (int i = Rows - 1; i >= 0; i--) {
    bool is_full = true;
    for (int j = 0; j < Columns; j++) {
      if (!Field[i][j].isFilled) {
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
      score += 1;
      i++;
    }
  }
}

Tetromino CurrentTetromino = Tetromino();
Tetromino NextTetromino = Tetromino();

void render(SDL_Renderer* renderer) {
  //Draw field
  for (int i = 0; i < Rows; i++) {
    for (int j = 0; j < Columns; j++) {
      SDL_Rect tile = { j * TitleSize + 2, i * TitleSize + 2, TitleSize - 4, TitleSize - 4 };
      SDL_SetRenderDrawColor(renderer, Field[i][j].color_r, Field[i][j].color_g, Field[i][j].color_b, 255);
      SDL_RenderFillRect(renderer, &tile);
    }
  }
  //Draw ghost
  CurrentTetromino.getGhost();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (CurrentTetromino.data.shape[i][j]) {
        SDL_SetRenderDrawColor(renderer, CurrentTetromino.data.color_r, CurrentTetromino.data.color_g, CurrentTetromino.data.color_b, 255);
        SDL_Rect outline = { (CurrentTetromino.ghost_x + j) * TitleSize + 2, (CurrentTetromino.ghost_y + i) * TitleSize + 2, TitleSize - 4, TitleSize - 4 };
        SDL_RenderFillRect(renderer, &outline);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect tile = { (CurrentTetromino.ghost_x + j) * TitleSize + 4, (CurrentTetromino.ghost_y + i) * TitleSize + 4, TitleSize - 9, TitleSize - 9 };
        SDL_RenderFillRect(renderer, &tile);
      }
    }
  }
  //Draw tetronimo
  if (!CurrentTetromino.isCollided()) {
    SDL_SetRenderDrawColor(renderer, CurrentTetromino.data.color_r, CurrentTetromino.data.color_g, CurrentTetromino.data.color_b, 255);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (CurrentTetromino.data.shape[i][j]) {
          SDL_Rect tile = { (CurrentTetromino.x + j) * TitleSize + 2, (CurrentTetromino.y + i) * TitleSize + 2, TitleSize - 4, TitleSize - 4 };
          SDL_RenderFillRect(renderer, &tile);
        }
      }
    }
  }
  
  //Draw Side Panel
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Rect side_panel = { Columns * TitleSize, 0, 200, 200 };
  SDL_RenderFillRect(renderer, &side_panel);

  //Draw next tetronimo
  SDL_SetRenderDrawColor(renderer, NextTetromino.data.color_r, NextTetromino.data.color_g, NextTetromino.data.color_b, 255);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (NextTetromino.data.shape[i][j]) {
        SDL_Rect tile = { (j + Columns + 2) * TitleSize - 20, (i + 2) * TitleSize, TitleSize - 4, TitleSize - 4 };
        SDL_RenderFillRect(renderer, &tile);
      }
    }
  }
  //Draw score
  
  //Present
  SDL_RenderPresent(renderer);
}

void update() {
  current_time = SDL_GetTicks();
  deltatime = current_time - last_time;
  destroyLine();
  if (deltatime > 500) {
    //Move down
    CurrentTetromino.y++;
    last_time = current_time;
    deltatime = 0;
  }
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

  TTF_Init();
  TTF_Font *font = TTF_OpenFont("arial.ttf", 50);
  TTF_Font *score_font = TTF_OpenFont("arial.ttf", 200);
  if (font == NULL) {
    std::cout << "Could not load font: " << TTF_GetError() << std::endl;
    return 1;
  }
  SDL_Surface *surface = TTF_RenderText_Solid(font, "Score", {255, 255, 255});
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect text_rect = { x_display, y_display, 100, 50 };
  SDL_RenderCopy(renderer, texture, NULL, &text_rect);
  SDL_FreeSurface(surface);

  bool is_running = true;
  while (is_running) {
    if (SDL_PollEvent(&window_event)) {
      if (window_event.type == SDL_QUIT || window_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        is_running = false;
      }
      if (window_event.type == SDL_KEYDOWN && !CurrentTetromino.isCollided()) {
        switch (window_event.key.keysym.scancode) {
          case SDL_SCANCODE_A:
            CurrentTetromino.x--;
            CurrentTetromino.ghost_x--;
            if (CurrentTetromino.isCollided()) {
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
    }

    update();
    render(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);

    //Display score
    std::string score_text = std::to_string(score);

    

    surface = TTF_RenderText_Solid(score_font, score_text.c_str(), {255, 255, 255});
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_rect = { x_display, y_display + 50, 100, 70 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &text_rect);
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_RenderPresent(renderer);

    if (Field[0][Columns / 2].isFilled && CurrentTetromino.isCollided()) {
      is_running = false;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return EXIT_SUCCESS;
}