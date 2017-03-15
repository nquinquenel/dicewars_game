#include <SDL2/SDL.h>
#include "generation.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

void generer_map(SDL_Renderer* renderer, int h, int w, int nbJoueurs) {
  int i;
  int p;
  int j;
  int couleur_actuelle = malloc(sizeof(int));
  couleur_actuelle = 0;
  srand(time(NULL));

  //SDL_RenderSetScale(renderer, 5.0, 5.0);

  int couleurs[8][3] = {{0,0,255}, {0, 255, 0}, {255, 0, 0}, {128, 255, 0}, {255, 128, 0}, {0, 255, 255}, {102, 51, 0}, {255, 102, 255}};

  //Tableau de points
  int** tab_points = malloc(30*sizeof(int*));
  int** tab_couleurs = malloc(30*sizeof(int*));


  for (i = 0; i < 40; i++) {
    tab_points[i] = malloc(2*sizeof(int));
    tab_couleurs[i] = malloc(3*sizeof(int));
    couleur_aleatoire(tab_couleurs, i, &couleur_actuelle, couleurs, nbJoueurs);
    point_aleatoire(h, w, tab_points, i);
  }

  int distanceMin = h;
  int distanceMin2 = h;
  int dist = 0;
  int couleur_point = 0;
  for (i = 0; i < h; i++) {
    for (p = 0; p < w; p++) {

      for (j = 0; j < 40; j++) {
        dist = sqrt(pow((i - tab_points[j][0]),2) + pow((p - tab_points[j][1]),2));
        if (dist < distanceMin) {
          distanceMin2 = distanceMin;
          distanceMin = dist;
          couleur_point = j%nbJoueurs;
        }
      }

      if ((distanceMin2 - distanceMin) < 4) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      } else {
        printf("%d\n",tab_couleurs[couleur_point][0]);
        SDL_SetRenderDrawColor(renderer, tab_couleurs[couleur_point][0], tab_couleurs[couleur_point][1], tab_couleurs[couleur_point][2], 255);
      }
      SDL_RenderDrawPoint(renderer, i, p);

      distanceMin = h;
      distanceMin2 = h;
      dist = 0;
      couleur_point = 0;
    }
  }

  // Render the rect to the screen
  SDL_RenderPresent(renderer);
}

//Génère un point aléatoire
void point_aleatoire(int h, int w, int **tab, int row) {
  tab[row][0] = rand()%(h);
  tab[row][1] = rand()%(w);
}

//Attribue une couleur à un point
void couleur_aleatoire(int **tab, int row, int *couleur_actuelle, int couleur[8][3], int nbJoueurs) {
  tab[row][0] = couleur[*couleur_actuelle][0];
  tab[row][1] = couleur[*couleur_actuelle][1];
  tab[row][2] = couleur[*couleur_actuelle][2];
  (*couleur_actuelle)++;
  if ((*couleur_actuelle) > nbJoueurs-1) {
    *couleur_actuelle = 0;
  }
}
