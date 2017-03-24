#include <SDL2/SDL.h>
#include "fenetre.h"
#include <stdio.h>
#include "jouer.h"

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

  //Tableau de comparaison entre id et joueurs
  int *tab_comparaison = malloc(50*sizeof(int));

  //Tableau des ID de chaques cellules
  int** tab_id = malloc(802*sizeof(int*));

  //Mise en place du renderer qui permet d'intérargir avec la fenêtre SDL2
  SDL_Renderer* renderer = NULL;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //Nettoie la fenêtre
  SDL_RenderClear(renderer);

  generer_map(renderer, 800, 600, 6, 50, tab_comparaison, tab_id);

  SDL_Event e;

  while (running == 1) {
    while(SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT:
        running = 0;
      case SDL_MOUSEBUTTONDOWN:
        attaquer_territoire(e.button.x, e.button.y, 800, 600, tab_comparaison, tab_id, renderer);
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
