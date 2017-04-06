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

  //Mise en place du renderer qui permet d'intérargir avec la fenêtre SDL2
  SDL_Renderer* renderer = NULL;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //Nettoie la fenêtre
  SDL_RenderClear(renderer);

  // positionnement et taille de l'image
  SDL_Rect img_pos;
  img_pos.x = 580;
  img_pos.y = 612;
  img_pos.w = 200;
  img_pos.h = 75;

  SDL_Surface* background_surface = SDL_LoadBMP("../Images/nouveau.bmp");
  SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
  SDL_FreeSurface(background_surface);
  SDL_RenderCopy(renderer, background_texture, NULL, &img_pos);

  SDL_Rect img_pos2;
  img_pos2.x = 360;
  img_pos2.y = 612;
  img_pos2.w = 200;
  img_pos2.h = 75;

  SDL_Surface* background_surface2 = SDL_LoadBMP("../Images/passer.bmp");
  SDL_Texture* background_texture2 = SDL_CreateTextureFromSurface(renderer, background_surface2);
  SDL_FreeSurface(background_surface2);
  SDL_RenderCopy(renderer, background_texture2, NULL, &img_pos2);

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

  int* tabPerdants = malloc(8*sizeof(int));
  for (i = 0; i < 8; i++) {
    tabPerdants[i] = -1;
  }

  //Tableau des couleurs disponibles
  int couleurs[8][3] = {{0,0,255}, {0, 255, 0}, {255, 0, 0}, {128, 68, 188}, {255, 128, 0}, {0, 255, 255}, {102, 51, 0}, {255, 102, 255}};
  printf("Joueur 1 : bleu, Joueur 2 : vert, Joueur 3 : rouge, Joueur 4 : Mauve, Joueur 5 : Orange, Joueur 6 : bleu clair\n");

  SMap *map;

  map = generer_map(renderer, 800, 600, nbJoueurs, 50, tab_comparaison, tab_id, tab_points);

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
  int nbTurn = 0;
  int IAPause = 0;
  int jeuFini = 0;
  char *output = {""};

  while (running == 1) {

    tourFini = 0;
    idJoueurActuel = getIdJoueurActuel();
    int cmptPerdants = 0;
    for (i = 0; i < nbJoueurs; i++) {
      if (tabPerdants[i] == 1) {
        cmptPerdants++;
      }
    }
    if (cmptPerdants == nbJoueurs-1) {
      jeuFini = 1;
    }

    if (jeuFini == 0) {
      //Au tour de l'IA de jouer
      if (isAnIA(idJoueurActuel) == 1) {
        STurn *turn = malloc(sizeof(turn));

        //Tant que l'IA n'a pas fini de jouer
        while (tourFini == 0) {
          if (running == 0) {
            break;
          }
          if (SDL_PollEvent(&e) != 0) {
            switch (e.type) {

              //On appuie sur la croix rouge de la fenêtre
              case SDL_QUIT:
              running = 0;
              break;

              //Clique souris
              case SDL_MOUSEBUTTONDOWN:
              if (e.button.x > 359 && e.button.x < 561 && e.button.y > 612 && e.button.y < 688) {
                if (IAPause == 0) {
                  IAPause = 1;
                } else {
                  IAPause = 0;
                }
                break;
              } else if (e.button.x > 579 && e.button.x < 781 && e.button.y > 612 && e.button.y < 688) {
                for (i = 0; i < 50; i++) {
                  free(tab_points[i]);
                }
                free(tab_points);

                for (i = 0; i < 799; i++) {
                  free(tab_borduresBlanches[i]);
                }
                free(tab_borduresBlanches);

                free(tab_comparaison);

                for (i = 0; i < 802; i++) {
                  free(tab_id[i]);
                }
                free(tab_id);
                //Tableau pour remettre les bordures blanches par défaut
                tab_id = malloc(802*sizeof(int*));
                tab_borduresBlanches = malloc(799*sizeof(int*));
                tab_points = malloc(50*sizeof(int*));
                tab_comparaison = malloc(50*sizeof(int));

                for (i = 0; i < 50; i++) {
                  tab_points[i] = malloc(2*sizeof(int));
                }

                for (i = 0; i < 799; i++) {
                  tab_borduresBlanches[i] = malloc(599*sizeof(int));
                  for (p = 0; p < 599; p++) {
                    tab_borduresBlanches[i][p] = 0;
                  }
                }

                idJoueurActuel = 0;
                map = generer_map(renderer, 800, 600, nbJoueurs, 50, tab_comparaison, tab_id, tab_points);

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

                tourFini = 1;
                break;
              }
            }
          } else if (IAPause == 0) {

            //L'IA joue son tour
            output=concat(output,"tour numero:");
            output=concatint(output,nbTurn);
            playIA = PlayTurn4(idJoueurActuel,map, turn);
            //Si elle souhaite attaquer
            if (playIA == 1) {
              output=concat(output, "joueur:");
              output=concatint(output, idJoueurActuel);
              output=concat(output,"attaque avec");
              output=concatint(output,GetCell(map,turn->cellFrom)->nbDices);
              output=concat(output,"contre:");
              output=concat(output,"joueur:");
              output=concatint(output,GetCell(map,turn->cellTo)->owner);
              output=concat(output,"avec");
              output=concatint(output,GetCell(map,turn->cellTo)->nbDices);

              playIA = demandeAttaque(map, turn, idJoueurActuel);
              id = turn->cellTo;
              int idJoueurDefense = (GetCell(map, id))->owner;
              //Si l'IA a gagné son attaque
              if (playIA == 1) {
                attaquer_territoireSansCoord(id,800, 600, tab_comparaison, tab_id, renderer, map, idJoueurActuel, couleurs);
                // maj de highestCluster des 2 joueurs dans la cas d'une attaque réussie
                UpdateHighestCluster(map, GetCell(map, turn->cellFrom), idJoueurActuel); //MAJ pour le joueur en attaque
                UpdateHighestCluster(map, NULL, idJoueurDefense); //MAj pour le joueur en défense
                output = concat(output,"gagnee  nouveau nb=");
                output=concatint(output,contexts[idJoueurActuel]->highestCluster);
                output=concat(output,"\n");

              }
              if(playIA==0){
                output=concat(output,"perdu nouveau nb=");
                output=concatint(output,contexts[idJoueurActuel]->highestCluster);
                output=concat(output,"\n");
              }
              //Si l'IA a fait une attaque autorisé (gagné ou perdue)
              if (playIA != -1) {
                update_affichage(map, tab_points[turn->cellFrom][0], tab_points[turn->cellFrom][1], 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

                //Si l'IA fait un mouvement interdit
              } else {
                tourFini = 1;
                DistributeDices(map);
                update_affichage(map, tab_points[turn->cellFrom][0], tab_points[turn->cellFrom][1], 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

                //On passe au joueur suivant
                output=concat(output,"invalid");
                output=concat(output,"\n");
                idJoueurActuel++;
                nbTurn++;
                setIdJoueurActuel(idJoueurActuel, nbJoueurs);
                idJoueurActuel = getIdJoueurActuel();
                int dicesIA = 0;
                int noPerd = 0;
                while (noPerd == 0) {
                  for (i = 0; i < 50; i++) {
                    if ((map->cells[i]).owner == idJoueurActuel) {
                      dicesIA += (map->cells[i]).nbDices;
                    }
                  }
                  if (dicesIA == 0) {
                    tabPerdants[idJoueurActuel] = 1;
                    idJoueurActuel++;
                    setIdJoueurActuel(idJoueurActuel, nbJoueurs);
                    idJoueurActuel = getIdJoueurActuel();
                    dicesIA = 0;
                  } else {
                    noPerd = 1;
                  }
                }

                printf("Au joueur %d de jouer(A)\n", getIdJoueurActuel());
              }
              //Si elle passe son tour
            } else {
              tourFini = 1;
              // on distribue aléatoirement les dés sur les territoires alliés
              DistributeDices(map);
              update_affichage(map, tab_points[turn->cellFrom][0], tab_points[turn->cellFrom][1], 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

              //On passe au joueur suivant
              idJoueurActuel++;
              nbTurn++;

              setIdJoueurActuel(idJoueurActuel, nbJoueurs);
              idJoueurActuel = getIdJoueurActuel();
              int dicesIA = 0;
              int noPerd = 0;
              while (noPerd == 0) {
                for (i = 0; i < 50; i++) {
                  if ((map->cells[i]).owner == idJoueurActuel) {
                    dicesIA += (map->cells[i]).nbDices;
                  }
                }
                if (dicesIA == 0) {
                  tabPerdants[idJoueurActuel] = 1;
                  idJoueurActuel++;
                  setIdJoueurActuel(idJoueurActuel, nbJoueurs);
                  idJoueurActuel = getIdJoueurActuel();
                  dicesIA = 0;
                } else {
                  noPerd = 1;
                }
              }
              printf("Au joueur %d de jouer(B)\n", getIdJoueurActuel());
            }
          }
        }

        //On free le turn
        free(turn);
        writetoLog(output);
        output="";
      }
    }

    while(SDL_PollEvent(&e) != 0) {

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
          } else if (e.button.x > 359 && e.button.x < 561 && e.button.y > 612 && e.button.y < 688) {
            DistributeDices(map);
            update_affichage(map, 0, 0, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);
            idJoueurActuel++;

            setIdJoueurActuel(idJoueurActuel, nbJoueurs);
            idJoueurActuel = getIdJoueurActuel();

            int dicesIA = 0;
            int noPerd = 0;
            while (noPerd == 0) {
              for (i = 0; i < 50; i++) {
                if ((map->cells[i]).owner == idJoueurActuel) {
                  dicesIA += (map->cells[i]).nbDices;
                }
              }
              if (dicesIA == 0) {
                tabPerdants[idJoueurActuel] = 1;
                printf("Joueur %d a perdu\n", idJoueurActuel);
                idJoueurActuel++;
                setIdJoueurActuel(idJoueurActuel, nbJoueurs);
                idJoueurActuel = getIdJoueurActuel();
                dicesIA = 0;
              } else {
                noPerd = 1;
              }
            }
            printf("Au joueur %d de jouer\n", getIdJoueurActuel());
            break;
          } else if (e.button.x > 579 && e.button.x < 781 && e.button.y > 612 && e.button.y < 688) {
            for (i = 0; i < 50; i++) {
              free(tab_points[i]);
            }
            free(tab_points);

            for (i = 0; i < 799; i++) {
              free(tab_borduresBlanches[i]);
            }
            free(tab_borduresBlanches);

            free(tab_comparaison);

            for (i = 0; i < 802; i++) {
              free(tab_id[i]);
            }
            free(tab_id);
            //Tableau pour remettre les bordures blanches par défaut
            tab_id = malloc(802*sizeof(int*));
            tab_borduresBlanches = malloc(799*sizeof(int*));
            tab_points = malloc(50*sizeof(int*));
            tab_comparaison = malloc(50*sizeof(int));

            for (i = 0; i < 50; i++) {
              tab_points[i] = malloc(2*sizeof(int));
            }

            for (i = 0; i < 799; i++) {
              tab_borduresBlanches[i] = malloc(599*sizeof(int));
              for (p = 0; p < 599; p++) {
                tab_borduresBlanches[i][p] = 0;
              }
            }

            idJoueurActuel = 0;
            map = generer_map(renderer, 800, 600, nbJoueurs, 50, tab_comparaison, tab_id, tab_points);

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

            break;
          }

          //Phase 1 -> Quand on sélectionne le territoire à attaquer
        } else {
          cellDeux = territoireSelec(e.button.x, e.button.y, tab_id);
          STurn *turn = malloc(sizeof(turn));
          turn->cellFrom = cellUn;
          turn->cellTo = cellDeux;
          output=concat(output,"tour numero:");
          output=concatint(output,nbTurn);
          if (cellDeux != -1) {
            int idJoueurDefense = (GetCell(map, cellDeux))->owner;
            output=concat(output, "joueur:");
            output=concatint(output, idJoueurActuel);
            output=concat(output,"attaque avec");
            output=concatint(output,GetCell(map,turn->cellFrom)->nbDices);
            output=concat(output,"contre:");
            output=concat(output,"joueur:");
            output=concatint(output,GetCell(map,turn->cellTo)->owner);
            output=concat(output,"avec");
            output=concatint(output,GetCell(map,turn->cellTo)->nbDices);
            //On fait une demande d'attaque et on attaque (1 = attaque gagné, 0 = attaque perdue, -1 = attaque non valide)
            res = demandeAttaque(map, turn, idJoueurActuel);
            //Si res == 1 alors on a gagné l'attaque, on change la couleur du territoire attaqué
            if (res == 1) {
              attaquer_territoire(e.button.x, e.button.y, 800, 600, tab_comparaison, tab_id, renderer, map, idJoueurActuel, couleurs);
              // maj de highestCluster des 2 joueurs dans la cas d'une attaque réussie
              UpdateHighestCluster(map, GetCell(map, cellUn), idJoueurActuel); //MAJ pour le joueur en attaque
              UpdateHighestCluster(map, NULL, idJoueurDefense); //MAj pour le joueur en défense
              output = concat(output,"gagnee  nouveau nb=");
              output=concatint(output,contexts[idJoueurActuel]->highestCluster);
              output=concat(output,"\n");
            }
            if (res==0){
              output=concat(output,"perdu nouveau nb=");
              output=concatint(output,contexts[idJoueurActuel]->highestCluster);
              output=concat(output,"\n");
            }
          }
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

          if (cellDeux == -1) {
            update_affichage(map, 0, 0, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);
          }
          if (res != -1) {
            update_affichage(map, tab_points[turn->cellFrom][0], tab_points[turn->cellFrom][1], 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);
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

          DistributeDices(map);
          update_affichage(map, 0, 0, 800, 600, tab_points, tab_borduresBlanches, tab_id, tab_comparaison, couleurs, renderer);

          //On passe au joueur suivant
          idJoueurActuel++;
          nbTurn++;

          setIdJoueurActuel(idJoueurActuel, nbJoueurs);
          idJoueurActuel = getIdJoueurActuel();
          int dicesIA = 0;
          int noPerd = 0;
          while (noPerd == 0) {
            for (i = 0; i < 50; i++) {
              if ((map->cells[i]).owner == idJoueurActuel) {
                dicesIA += (map->cells[i]).nbDices;
              }
            }
            if (dicesIA == 0) {
              tabPerdants[idJoueurActuel] = 1;
              printf("Joueur %d a perdu\n", idJoueurActuel);
              idJoueurActuel++;
              setIdJoueurActuel(idJoueurActuel, nbJoueurs);
              idJoueurActuel = getIdJoueurActuel();
              dicesIA = 0;
            } else {
              noPerd = 1;
            }
          }
          printf("Au joueur %d de jouer\n", getIdJoueurActuel());
          writetoLog(output);
          output="";
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
  img_pos.y = (pixel_y-59);
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

void update_affichage(SMap* map, int x, int y, int h, int w, int** tab_points, int** tab_borduresBlanches, int** tab_id, int* tab_comparaison, int couleurs[8][3], SDL_Renderer* renderer) {
  int i, j;
  for (i = (x-150); i < (x+150); i++) {
    for (j = (y-150); j < (y+150); j++) {
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


void writetoLog(char *s){
  FILE *logfile = fopen("logfile.txt","a");
  if(logfile==NULL){
    printf("cant open file");
  }
  fprintf(logfile,"%s",s);
  fclose(logfile);

}

char* concat( char *s1,  char *s2)
{
  char *finals = malloc(strlen(s1)+strlen(s2)+3);//+1 for the zero-terminator
  strcpy(finals, s1);
  strcat(finals, s2);
  strcat(finals,"\t");
  return finals;
}

char* concatint(char *s1, int n)
{
  char tempbuff[INT_BUFFER_SIZE];
  sprintf(tempbuff,"%d",n);
  char *finals = malloc(strlen(s1)+strlen(tempbuff)+3);//+1 for the zero-terminator
  strcpy(finals, s1);
  strcat(finals, tempbuff);
  strcat(finals,"\t");
  return finals;
}

//COPYRIGHT Nicolas
