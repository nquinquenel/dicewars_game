#include <SDL2/SDL.h>
#include "fenetre.h"
#include <stdio.h>

void fenetre()
{
  SDL_Window* window = NULL;
  window = SDL_CreateWindow
  (
      "DiceWars", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      SDL_WINDOW_SHOWN
  );

  // Setup renderer
  SDL_Renderer* renderer = NULL;

  renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
  // Set render color to red ( background will be rendered in this color )
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

  // Clear winow
  SDL_RenderClear( renderer );

  generer_map(renderer, 800, 600, 4);

  // Wait for 5 sec
  SDL_Delay( 5000 );

  SDL_DestroyWindow(window);
  SDL_Quit();
}
