#include <SDL2/SDL.h>
#include "fenetre.h"
#include <stdio.h>

/********************************************************************************************
*
* FUNCTION NAME: fenetre
*
* DESCRIPTION: Génère la fenêtre de jeu SDL2 et départ du programme (boucle du jeu)
*
*********************************************************************************************/
void fenetre() {
  //Condition pour faire tourner le jeu en boucle
  int running = 1;
  SDL_Window* window = NULL;
  window = SDL_CreateWindow
  (
      "DiceWars", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      SDL_WINDOW_SHOWN
  );

  //Mise en place du renderer qui permet d'intérargir avec la fenêtre SDL2
  SDL_Renderer* renderer = NULL;

  renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

  //Nettoie la fenêtre
  SDL_RenderClear( renderer );

  generer_map(renderer, 800, 600, 6, 50);

  SDL_Event e;

  while (running == 1) {
    while( SDL_PollEvent( &e ) != 0 ){
      if(e.type == SDL_QUIT){
        running = 0;
        printf("Closing the window ...\n");
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
