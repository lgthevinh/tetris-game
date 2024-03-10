#include <iostream>
#include <SDL2/SDL.h>

const int TitleSize = 30; // 10 pixels
const int Rows = 20, Columns = 10;

const int WIDTH = Columns * TitleSize;
const int HEIGHT = Rows * TitleSize;

bool Field[Rows][Columns] = {0};

struct Tetronimo {
  int pos_x, pos_y;
  const int* shape;
  int size;
};

Tetronimo currentTetronimo;

const int Tetronimo_I[] = {
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,
  0, 0, 0, 0
};

const int Tetronimo_O[] = {
  1, 1,
  1, 1
};

const int Tetronimo_T[] = {
  0, 1, 0,
  1, 1, 1,
};

const int Tetronimo_S[] = {
  0, 1, 1,
  1, 1, 0,
  0, 0, 0
};

const int Tetronimo_Z[] = {
  1, 1, 0,
  0, 1, 1,
  0, 0, 0
};

const int Tetronimo_J[] = {
  1, 0, 0,
  1, 1, 1,
  0, 0, 0
};

const int Tetronimo_L[] = {
  0, 0, 1,
  1, 1, 1,
  0, 0, 0
};

const Tetronimo Tetronimos[] = {
  {0, 0, Tetronimo_I, 4},
  {0, 0, Tetronimo_O, 2},
  {0, 0, Tetronimo_T, 3},
  {0, 0, Tetronimo_S, 3},
  {0, 0, Tetronimo_Z, 3},
  {0, 0, Tetronimo_J, 3},
  {0, 0, Tetronimo_L, 3}
};

void update() {
  //Update
  if(currentTetronimo.pos_y < Rows) {
    currentTetronimo.pos_y++;
  }
  else {
    //Place the tetronimo on the field
    for (int i = 0; i < currentTetronimo.size; i++) {
      for (int j = 0; j < currentTetronimo.size; j++) {
        if (currentTetronimo.shape[i * currentTetronimo.size + j]) {
          Field[currentTetronimo.pos_y + i][currentTetronimo.pos_x + j] = 1;
        }
      }
    }
    //Spawn a new tetronimo
    currentTetronimo = Tetronimos[rand() % 7];
  }
} 

void render(SDL_Renderer* renderer) {
  //Draw background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  //Draw field
  for (int i = 0; i < Rows; i++) {
    for (int j = 0; j < Columns; j++) {
      if (Field[i][j]) {
        SDL_Rect tile = {j * TitleSize+2, i * TitleSize+2, TitleSize-4, TitleSize-4};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &tile);
      }
    }
  }
  //Draw tetronimo
  
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
      
      render(renderer);
    }
  }
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}