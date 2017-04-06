#include <SDL2/SDL.h>
#include "fenetre.h"
#include <stdio.h>
#include "jouer.h"
#include "../Librairies/interfacePerso.h"

int IMG_DICES_W = 34;//la dimension de l'image des dés en largeur
int IMG_DICES_H = 59; //la dimension de l'image des dés en hauteur

/********************************************************************************************
*
* FUNCTION NAME: fenetre
*
* DESCRIPTION: Génère la fenêtre de jeu SDL2 et départ du programme (boucle du jeu)
*
*********************************************************************************************/
void fenetre(int nbJoueurs) {
  //Condition pour faire tourner le jeu en boucle
  int running = 1;
  int i, p, s;
  SDL_Window* window = NULL;
  window = SDL_CreateWindow
  (
    "DiceWars", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    800,
    700,
    SDL_WINDOW_SHOWN
  );

  //Tableau de comparaison entre id et joueurs
  int *tab_comparaison = malloc(50*sizeof(int));

  //Tableau des ID de chaques cellules
  int** tab_id = malloc(802*sizeof(int*));

  //Tableau pour remettre les bordures blanches par défaut
  int** tab_borduresBlanches = malloc(799*sizeof(int*));

  //Tableau de points
  int** tab_points = malloc(50*sizeof(int*));

  for (i = 0; i < 50; i++) {
    tab_points[i] = malloc(2*sizeof(int));
  }

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

  map = generer_map(renderer, 800, 600, nbJoueurs, 50, tab_comparaison, tab_id, tab_points);

  // allocation mémoire pour 2 tableaux de pointeurs pour gérer l'affichage des images de dés des cellules
  // Tableau de pointeurs de SDL_Surface pour les images de dés
  //SDL_Surface** background_surface_tab = malloc(50*sizeof(SDL_Surface *));
  // Tableau de pointeurs de SDL_Texture pour les images de dés
  //SDL_Texture** background_texture_tab = malloc(50*sizeof(SDL_Texture *));

  // initialisation de highestCluster de la struct SContext
  SContext **contexts = GetContexts();
  for (s = 0; s < nbJoueurs; s++) { //pour chaque joueur
    int t;
    int clusterSize = 0; //la taille de la plus grosse grappe
    SCell *allCells = map->cells; // toutes les cellules de la map
    for (t = 0; t < (map->nbCells); t++) { //parcours des cellules de la map pour récupérer la taille de la plus grosse grappe de cellules
      if(allCells[t].owner == s) { //si le joueur est le propriétaire de la cellule
        int tmp = GetClusterSize(map, &allCells[t]);
        if (tmp > clusterSize) clusterSize = tmp;
      }
    }
    contexts[s]->highestCluster = clusterSize;
  }

  SDL_Event e;

  int res, cellUn, cellDeux, idJoueurActuel, playIA, id;
  int tourFini = 0;
  int phase = 0;

  while (running == 1) {
    while(SDL_PollEvent(&e) != 0) {
      tourFini = 0;
      idJoueurActuel = getIdJoueurActuel();

      //Au tour de l'IA de jouer
      if (isAnIA(idJoueurActuel) == 1) {
        STurn *turn = malloc(sizeof(turn));

        //Tant que l'IA n'a pas fini de jouer
        while (tourFini == 0) {
          //L'IA joue son tour
          playIA = PlayTurn(map, turn);
          //Si elle souhaite attaquer
          if (playIA == 1) {
            playIA = demandeAttaque(map, turn, idJoueurActuel);
            id = turn->cellTo;
            int idJoueurDefense = (GetCell(map, id))->owner;
            //Si l'IA a gagné son attaque
            if (playIA == 1) {
              attaquer_territoireSansCoord(id,800, 600, tab_comparaison, tab_id, renderer, map, idJoueurActuel, couleurs);
              // maj de highestCluster des 2 joueurs dans la cas d'une attaque réussie
              UpdateHighestCluster(map, GetCell(map, turn->cellFrom), idJoueurActuel); //MAJ pour le joueur en attaque
              UpdateHighestCluster(map, NULL, idJoueurDefense); //MAj pour le joueur en défense
            }

            //Si l'IA a fait une attaque autorisé (gagné ou perdue)
            if (playIA != -1) {
              /*int nbDes;
              nbDes = map->cells[id].nbDices;
              displayDices(renderer, tab_points[id][0], tab_points[id][1], id, nbDes);
              id = turn->cellFrom;
              nbDes = map->cells[id].nbDices;
              displayDices(renderer, tab_points[id][0], tab_points[id][1], id, nbDes);
              */
              update_affichage(map, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

              //Si l'IA fait un mouvement interdit
            } else {
              tourFini = 1;
              DistributeDices(map);
              /*int z;
              for (z = 0; z < map->nbCells; z++) {
                if ((map->cells[z]).owner == idJoueurActuel) {
                  displayDices(renderer, tab_points[(map->cells[z]).id][0], tab_points[(map->cells[z]).id][1], (map->cells[z]).id, (map->cells[z]).nbDices);
                }
              }
              SDL_RenderPresent(renderer);*/
              update_affichage(map, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

              //On passe au joueur suivant
              idJoueurActuel++;
              setIdJoueurActuel(idJoueurActuel, nbJoueurs);
              printf("Au joueur %d de jouer\n", getIdJoueurActuel());
            }
            //Si elle passe son tour
          } else {
            tourFini = 1;
            // on distribue aléatoirement les dés sur les territoires alliés
            DistributeDices(map);
          /*  int z;
            for (z = 0; z < map->nbCells; z++) {
              if ((map->cells[z]).owner == idJoueurActuel) {
                displayDices(renderer, tab_points[(map->cells[z]).id][0], tab_points[(map->cells[z]).id][1], (map->cells[z]).id, (map->cells[z]).nbDices);
              }
            }
            SDL_RenderPresent(renderer);*/
            update_affichage(map, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

            //On passe au joueur suivant
            idJoueurActuel++;
            setIdJoueurActuel(idJoueurActuel, nbJoueurs);
            printf("Au joueur %d de jouer\n", getIdJoueurActuel());
          }
        }

        //On free le turn
        free(turn);
      }

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
          int idJoueurDefense = (GetCell(map, cellDeux))->owner;

          //On fait une demande d'attaque et on attaque (1 = attaque gagné, 0 = attaque perdue, -1 = attaque non valide)
          res = demandeAttaque(map, turn, idJoueurActuel);
          //Si res == 1 alors on a gagné l'attaque, on change la couleur du territoire attaqué
          if (res == 1) {
            attaquer_territoire(e.button.x, e.button.y, 800, 600, tab_comparaison, tab_id, renderer, map, idJoueurActuel, couleurs);
            // maj de highestCluster des 2 joueurs dans la cas d'une attaque réussie
            UpdateHighestCluster(map, GetCell(map, cellUn), idJoueurActuel); //MAJ pour le joueur en attaque
            UpdateHighestCluster(map, NULL, idJoueurDefense); //MAj pour le joueur en défense
          }

          /*int nbDes;
          nbDes = map->cells[cellUn].nbDices;
          displayDices(renderer, tab_points[cellUn][0], tab_points[cellUn][1], cellUn, nbDes);
          nbDes = map->cells[cellDeux].nbDices;
          displayDices(renderer, tab_points[cellDeux][0], tab_points[cellDeux][1], cellDeux, nbDes);
*/
          //On enlève les bordures internes blanches de notre territoire
          for (i = 1; i < 799; i++) {
            for (p = 1; p < 599; p++) {
              if (tab_borduresBlanches[i][p] == 1) {
                if (res == -1) {
                  SDL_SetRenderDrawColor(renderer, couleurs[idJoueurActuel][0], couleurs[idJoueurActuel][1], couleurs[idJoueurActuel][2], 0);
                  SDL_RenderDrawPoint(renderer, i, p);
                }
                tab_borduresBlanches[i][p] = 0;
              }
            }
          }

          if (res != -1) {
            update_affichage(map, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);
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

          // TODO appliquer la distribution des dés sur l'interface

          //On enlève les bordures internes blanches
        /*  if (phase == 1) {
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
          }*/

          update_affichage(map, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

        /*  // on distribue aléatoirement les dés sur les territoires alliés
          DistributeDices(map);
          int z;
          for (z = 0; z < map->nbCells; z++) {
            if ((map->cells[z]).owner == idJoueurActuel) {
              displayDices(renderer, tab_points[(map->cells[z]).id][0], tab_points[(map->cells[z]).id][1], (map->cells[z]).id, (map->cells[z]).nbDices);
            }
          }
          SDL_RenderPresent(renderer);
*/
          //On passe au joueur suivant
          idJoueurActuel++;
          setIdJoueurActuel(idJoueurActuel, nbJoueurs);
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

/********************************************************************************************
*
* FUNCTION NAME: displayDices
*
* DESCRIPTION: affichage de l'image des dés de la cellule
*
* ARGUMENT                  TYPE             DESCRIPTION
* renderer                  SDL_Renderer*    Permet d'intérargir avec la fenêtre SDL2
* background_surface_tab    SDL_Surface**    Tableau de pointeurs de SDL_Surface pour les images de dés
* background_texture_tab    SDL_Texture**    Tableau de pointeurs de SDL_Texture pour les images de dés
* pixel_x                   int              coordonnée x du pixel central de la cellule
* pixel_y                   int              coordonnée y du pixel central de la cellule
* idCEll                    int              l'id de la cellule
* nbDices                   int              le nombre de dés de la cellule
*
*********************************************************************************************/
void displayDices(SDL_Renderer* renderer, int pixel_x, int pixel_y, int idCell, int nbDices) {
  // les objets SDL pour l'image
  SDL_Surface* background_surface = NULL;
  SDL_Texture* background_texture = NULL;

  // positionnement et taille de l'image
  SDL_Rect img_pos;
  img_pos.x = pixel_x;
  img_pos.y = (pixel_y-52);
  img_pos.w = IMG_DICES_W;
  img_pos.h = IMG_DICES_H;

  switch (nbDices) {
    case 1:
    background_surface = SDL_LoadBMP("../Images/1dés.bmp");
    break;
    case 2:
    background_surface = SDL_LoadBMP("../Images/2dés.bmp");
    break;
    case 3:
    background_surface = SDL_LoadBMP("../Images/3dés.bmp");
    break;
    case 4:
    background_surface = SDL_LoadBMP("../Images/4dés.bmp");
    break;
    case 5:
    background_surface = SDL_LoadBMP("../Images/5dés.bmp");
    break;
    case 6:
    background_surface = SDL_LoadBMP("../Images/6dés.bmp");
    break;
    case 7:
    background_surface = SDL_LoadBMP("../Images/7dés.bmp");
    break;
    case 8:
    background_surface = SDL_LoadBMP("../Images/8dés.bmp");
    break;
  }

  Uint32 colorkey = SDL_MapRGB(background_surface->format, 255, 255, 255);
  SDL_SetColorKey(background_surface, SDL_TRUE, colorkey);

  background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);

  SDL_FreeSurface(background_surface);

  //background_surface_tab[idCell] = background_surface;
  //background_texture_tab[idCell] = background_texture;
  SDL_RenderCopy(renderer, background_texture, NULL, &img_pos);
}

void update_affichage(SMap* map, int h, int w, int** tab_points, int** tab_borduresBlanches, int** tab_id, int* tab_comparaison, int couleurs[8][3], SDL_Renderer* renderer) {
  int i, j;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      if (i > 0 && i < h && j > 0 && j < w) {
        if (tab_id[i+1][j+1] == -1) {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        } else {
          SDL_SetRenderDrawColor(renderer, couleurs[tab_comparaison[tab_id[i+1][j+1]]][0], couleurs[tab_comparaison[tab_id[i+1][j+1]]][1], couleurs[tab_comparaison[tab_id[i+1][j+1]]][2], 0);
        }
        SDL_RenderDrawPoint(renderer, i, j);
      }
    }
  }
  for (i = 0; i < map->nbCells; i++) {
    displayDices(renderer, tab_points[(map->cells[i]).id][0], tab_points[(map->cells[i]).id][1], (map->cells[i]).id, (map->cells[i]).nbDices);
  }
  SDL_RenderPresent(renderer);
}

//COPYRIGHT Nicolas
