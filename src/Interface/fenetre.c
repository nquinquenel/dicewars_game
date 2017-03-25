#include <SDL2/SDL.h>
#include "fenetre.h"
#include <stdio.h>
#include "jouer.h"
#include "../Librairies/interfacePerso.h"

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
  int i, p;
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

  //Tableau pour remettre les bordures blanches par défaut
  int** tab_borduresBlanches = malloc(799*sizeof(int*));

  for (i = 0; i < 799; i++) {
    tab_borduresBlanches[i] = malloc(599*sizeof(int));
    for (p = 0; p < 599; p++) {
      tab_borduresBlanches[i][p] = 0;
    }
  }

  //Tableau des couleurs disponibles
  int couleurs[8][3] = {{0,0,255}, {0, 255, 0}, {255, 0, 0}, {128, 68, 188}, {255, 128, 0}, {0, 255, 255}, {102, 51, 0}, {255, 102, 255}};
  printf("Joueur 1 : bleu, Joueur 2 : vert, Joueur 3 : rouge, Joueur 4 : Mauve, Joueur 5 : Orange, Joueur 6 : bleu clair\n");

  //Mise en place du renderer qui permet d'intérargir avec la fenêtre SDL2
  SDL_Renderer* renderer = NULL;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //Nettoie la fenêtre
  SDL_RenderClear(renderer);

  SMap *map;

  map = generer_map(renderer, 800, 600, 6, 50, tab_comparaison, tab_id);

  SDL_Event e;

  int res, cellUn, cellDeux, idJoueurActuel;
  int phase = 0;
  while (running == 1) {
    while(SDL_PollEvent(&e) != 0) {
      idJoueurActuel = getIdJoueurActuel();
      switch (e.type) {

      //On appuie sur la croix rouge de la fenêtre
      case SDL_QUIT:
        running = 0;
        break;

      //Clique souris
      case SDL_MOUSEBUTTONDOWN:
        //Phase 0 -> Quand on sélectionne son territoire
        if (phase == 0) {
          //On récupère l'ID du territoire sélectionné
          cellUn = territoireSelec(e.button.x, e.button.y, tab_id);
          //On regarde si c'est notre territoire, si oui on met une bordure interne blanche
          if (cellUn != -1 && tab_comparaison[cellUn] == idJoueurActuel) {
            notifTerrains(cellUn, tab_id, renderer, 800, 600, tab_borduresBlanches);
            phase = 1;
          }

        //Phase 1 -> Quand on sélectionne le territoire à attaquer
        } else {
          cellDeux = territoireSelec(e.button.x, e.button.y, tab_id);
          STurn *turn = malloc(sizeof(turn));
          turn->cellFrom = cellUn;
          turn->cellTo = cellDeux;
          //On fait une demande d'attaque et on attaque (1 = attaque gagné, 0 = attaque perdue, -1 = attaque non valide)
          res = demandeAttaque(map, turn, idJoueurActuel);
          //Si res == 1 alors on a gagné l'attaque, on change la couleur du territoire attaqué
          if (res == 1) {
            attaquer_territoire(e.button.x, e.button.y, 800, 600, tab_comparaison, tab_id, renderer, map, idJoueurActuel, couleurs);
          }

          //On enlève les bordures internes blanches de notre territoire
          for (i = 1; i < 799; i++) {
            for (p = 1; p < 599; p++) {
              if (tab_borduresBlanches[i][p] == 1) {
                SDL_SetRenderDrawColor(renderer, couleurs[idJoueurActuel][0], couleurs[idJoueurActuel][1], couleurs[idJoueurActuel][2], 0);
                SDL_RenderDrawPoint(renderer, i, p);
                tab_borduresBlanches[i][p] = 0;
              }
            }
          }
          SDL_RenderPresent(renderer);

          //On free le turn
          free(turn);
          //On repasse à la phase 1
          phase = 0;
        }
        break;

      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
          //Touche entrée
          case SDLK_RETURN:

            //On enlève les bordures internes blanches
            if (phase == 1) {
              for (i = 1; i < 799; i++) {
                for (p = 1; p < 599; p++) {
                  if (tab_borduresBlanches[i][p] == 1) {
                    SDL_SetRenderDrawColor(renderer, couleurs[idJoueurActuel][0], couleurs[idJoueurActuel][1], couleurs[idJoueurActuel][2], 0);
                    SDL_RenderDrawPoint(renderer, i, p);
                    tab_borduresBlanches[i][p] = 0;
                  }
                }
              }
              SDL_RenderPresent(renderer);
              phase = 0;
            }

            //On passe au joueur suivant
            idJoueurActuel++;
            setIdJoueurActuel(idJoueurActuel, 6);
            printf("Au joueur %d de jouer\n", getIdJoueurActuel());

            break;
        }
        break;
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
