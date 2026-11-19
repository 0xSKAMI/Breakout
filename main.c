#include "SDL3/SDL.h"
#include "pthread.h"
#include "stdio.h"
#include <unistd.h>

#define HEIGHT 600
#define WIDTH 600
#define TITLE "Breakout"
#define BALL_SIZE 20.0
#define PADLE_SIZE 20.0
#define PADLE_LENGTH 100.0
#define PADLE_THICCNESS
#define PADLE_PADDING 50.0

float padle_x = WIDTH/2 - PADLE_LENGTH/2;
float padle_y = HEIGHT - PADLE_PADDING;
float ball_x = WIDTH/2 - BALL_SIZE/2;
float ball_y = HEIGHT/2 - BALL_SIZE/2;
float k_x = 1; 
float k_y = 1;

void *testing(void* arg) {
  const bool* something = SDL_GetKeyboardState(NULL);
  while(1) {
    usleep(1000);
    SDL_PumpEvents();
    if(something[SDL_SCANCODE_RIGHT] && padle_x + PADLE_LENGTH <= WIDTH - 1) {
      padle_x++;
    };
    if(something[SDL_SCANCODE_LEFT] && padle_x >= 1) {
      padle_x--;
    };
  };
};

int main() {
  pthread_t keyThread;
  
  if(!SDL_Init(SDL_INIT_VIDEO)){
    fprintf(stderr, "ERROR: There was an init error %s\n", SDL_GetError());
    return 1;
  };
  SDL_Window *displayId = NULL;
  SDL_Renderer *renderer = NULL;

  if(!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, SDL_WINDOW_VULKAN, &displayId, &renderer)){
    fprintf(stderr, "ERROR: Could not create renderer or window  %s\n", SDL_GetError());
    return 1;
  };

  if(!SDL_SetRenderVSync(renderer, 1)){
    fprintf(stderr, "ERROR: Could not do Vsync %s\n", SDL_GetError());
    return 1;
  };
  
  if(!SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0)){
    fprintf(stderr, "ERROR: Could not set draw color  %s\n", SDL_GetError());
    return 1;
  };
  
  if(!SDL_RenderClear(renderer)){
    fprintf(stderr, "ERROR: Could not clear window %s\n", SDL_GetError());
    return 1;
  };

  SDL_Event event;
  
  pthread_create(&keyThread, NULL, testing, &event);
  //  pthread_join(keyThread, NULL);

  bool quit = false;
  while (!quit){
    if(!SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0)){
      fprintf(stderr, "ERROR: Could not set draw color  %s\n", SDL_GetError());
      return 1;
    };
    
    if(!SDL_RenderClear(renderer)){
      fprintf(stderr, "ERROR: Could not clear window %s\n", SDL_GetError());
      return 1;
    };
    
    if(!SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)){
      fprintf(stderr, "ERROR: Could not set draw color  %s\n", SDL_GetError());
      return 0;
    };

    ball_x = ball_x + (1 * k_x);
    ball_y = ball_y - (1 * k_y);
    
    if(ball_x + BALL_SIZE >= WIDTH || ball_x <= 0){
      k_x  = k_x * -1;
    };

    if(ball_y <= 0) {
      k_y  = k_y * -1;
    };



    struct SDL_FRect padle = {
      .x = padle_x,
      .y = padle_y,
      .w = PADLE_LENGTH,
      .h = PADLE_SIZE,
    };

    struct SDL_FRect ball = {
      .x = ball_x,
      .y = ball_y,
      .w = BALL_SIZE,
      .h = BALL_SIZE,
    };
      
    if(!SDL_RenderFillRect(renderer, &padle)){
      fprintf(stderr, "ERROR: Could not clear window %s\n", SDL_GetError());
      return 1;
    };

    if(!SDL_RenderFillRect(renderer, &ball)){
      fprintf(stderr, "ERROR: Could not clear window %s\n", SDL_GetError());
      return 1;
    };

    if(!SDL_RenderPresent(renderer)){
      fprintf(stderr, "ERROR: Could not present rendered object onto the screen %s\n", SDL_GetError());
      return 1;
    };
  };
  
  SDL_Quit();
  return 0;
};
