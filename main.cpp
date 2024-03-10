#include <iostream>
#include <SDL2/SDL.h>

const int TitleSize = 30; // 10 pixels
const int Rows = 20, Columns = 10;

const int WIDTH = Columns * TitleSize;
const int HEIGHT = Rows * TitleSize;

int deltaTime = 0; // 1 second = 1000 milliseconds;
int currentTime = SDL_GetTicks();
int lastTime = currentTime;

bool Field[Rows][Columns] = {0};

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
      x = 0;
      y = 0;
      getData();
    }
    void getData() {
      data = Tetrominos[rand() % 7];
    }
    bool isCollided() {
      if (x < 0 || x > Columns || y > Rows) {
        return true;
      }
      return false;
    }
};

Tetromino currentTetromino = Tetromino();

void update() {
  currentTime = SDL_GetTicks();
  deltaTime = currentTime - lastTime;
  if (currentTetromino.isCollided()) {
    currentTetromino = Tetromino();
  }
  if (deltaTime > 1000) {
    //Move down
    currentTetromino.y++;
    lastTime = currentTime;
    std::cout << "X: " << currentTetromino.x << " Y: " << currentTetromino.y << std::endl;
    deltaTime = 0;
  }
} 

void render(SDL_Renderer* renderer) {
  //Draw background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  //Draw field
  for (int i = 0; i < Rows; i++) {
    for (int j = 0; j < Columns; j++) {
      if (!Field[i][j]) {
        SDL_Rect tile = {j * TitleSize+2, i * TitleSize+2, TitleSize-4, TitleSize-4};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &tile);
      }
    }
  }
  //Draw tetronimo
  SDL_SetRenderDrawColor(renderer, currentTetromino.data.color_r, currentTetromino.data.color_g, currentTetromino.data.color_b, 255);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (currentTetromino.data.shape[i][j]) {
        SDL_Rect tile = {(currentTetromino.x + j) * TitleSize+2, (currentTetromino.y + i) * TitleSize+2, TitleSize-4, TitleSize-4};
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
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool is_running = true;
  while (is_running) {
    if (SDL_PollEvent(&window_event)) {
      if (window_event.type == SDL_QUIT|| window_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        is_running = false;
      }
    }
    update();
    render(renderer);
  }
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}