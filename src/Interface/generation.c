#include <SDL2/SDL.h>
#include "generation.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "../Librairies/interface.h"

/********************************************************************************************
*
* FUNCTION NAME: generer_map
*
* DESCRIPTION: Génère la carte du jeu
*
* ARGUMENT      TYPE             DESCRIPTION
* renderer      SDL_Renderer*    Permet d'intérargir avec la fenêtre SDL2
* h             int              Hauteur de la fenêtre
* w             int              Largeur de la fenêtre
* nbJoueurs     int              Nombre de joueurs
* nbTerritoires int              Nombre de cellules
*
*********************************************************************************************/
SMap* generer_map(SDL_Renderer* renderer, int h, int w, int nbJoueurs, int nbTerritoires, int *tab_comparaison, int** tab_id) {

  //Itérateurs
  int i, p, j;

  //Index qui permet de savoir quelle est la couleur actuellement utilisé lors de la génération des couleurs aux joueurs
  int couleur_actuelle = 0;

  //Permet de générer une map aléatoire
  srand(time(NULL));
  //SDL_RenderSetScale(renderer, 5.0, 5.0);

  //Permet d'enlever les id aux pixels bordures (à la fin)
  int** id_tmp = malloc((h+2)*sizeof(int*));

  //Tableau des voisins de chaque territoire
  int** tab_voisins = malloc(nbTerritoires*sizeof(int*));


    //Tableau des couleurs disponibles
    int couleurs[8][3] = {{0,0,255}, {0, 255, 0}, {255, 0, 0}, {128, 68, 188}, {255, 128, 0}, {0, 255, 255}, {102, 51, 0}, {255, 102, 255}};

  //h+2 car on ne va pas utiliser la première valeur
  for (i = 0; i < h+2; i++) {
    tab_id[i] = malloc((w)*sizeof(int*));
    id_tmp[i] = malloc((w)*sizeof(int*));
  }

  //Initialise les valeurs à -1 pour éviter de tomber sur des adresses mémoires
  for (i = 0; i < h+2; i++) {
    for (p = 0; p < w+2; p++) {
      tab_id[i][p] = -1;
    }
  }

  //Tableau de points
  int** tab_points = malloc(nbTerritoires*sizeof(int*));
  int** tab_couleurs = malloc(nbTerritoires*sizeof(int*));

  //Initalise certains tableaux et génère les points ainsi que les couleurs
  for (i = 0; i < nbTerritoires; i++) {
    tab_points[i] = malloc(2*sizeof(int));
    tab_couleurs[i] = malloc(3*sizeof(int));
    couleur_aleatoire(tab_couleurs, i, &couleur_actuelle, couleurs, nbJoueurs);
    point_aleatoire(h, w, tab_points, i);
    tab_voisins[i] = malloc(nbTerritoires*sizeof(int));
    //J'initialise les voisins à -1
    for (p = 0; p < nbTerritoires; p++) {
      tab_voisins[i][p] = -1;
    }
  }

  float distanceMin = h;
  float dist = 0;
  int couleur_point = 0;
  int id = 0;
  i = 0;
  p = 0;
  for (i = 0; i < h; i++) {
    for (p = 0; p < w; p++) {

      for (j = 0; j < nbTerritoires; j++) {
        dist = fabs(i - tab_points[j][0]) + fabs(p - tab_points[j][1]);
        if (dist < distanceMin) {
          distanceMin = dist;
          id = j;
          couleur_point = j%nbJoueurs;
        }
      }

      //Attribue au pixel l'id du point auquel il appartient
      tab_id[i+1][p+1] = id;
      tab_comparaison[id] = id%nbJoueurs;

      SDL_SetRenderDrawColor(renderer, tab_couleurs[couleur_point][0], tab_couleurs[couleur_point][1], tab_couleurs[couleur_point][2], 0);
      SDL_RenderDrawPoint(renderer, i, p);

      distanceMin = h;
      dist = 0;
      couleur_point = 0;
      id = 0;
    }
  }

  //Gestion des bordures et des voisins
  int bordure = 0;
  for (i = 0; i < h; i++) {
    for (p = 0; p < w; p++) {
      int id_cel = tab_id[i+1][p+1];
      if (tab_id[i+1][p] != id_cel && tab_id[i+1][p] != -1) {
        tab_voisins[id_cel][tab_id[i+1][p]] = tab_id[i+1][p];
        tab_voisins[tab_id[i+1][p]][id_cel] = id_cel;
        bordure = 1;
      } else if (tab_id[i+1][p+2] != id_cel && tab_id[i+1][p+2] != -1) {
        tab_voisins[id_cel][tab_id[i+1][p+2]] = tab_id[i+1][p+2];
        tab_voisins[tab_id[i+1][p+2]][id_cel] = id_cel;
        bordure = 1;
      } else if (tab_id[i][p+1] != id_cel && tab_id[i][p+1] != -1) {
        tab_voisins[id_cel][tab_id[i][p+1]] = tab_id[i][p+1];
        tab_voisins[tab_id[i][p+1]][id_cel] = id_cel;
        bordure = 1;
      } else if (tab_id[i+2][p+1] != id_cel && tab_id[i+2][p+1] != -1) {
        tab_voisins[id_cel][tab_id[i+2][p+1]] = tab_id[i+2][p+1];
        tab_voisins[tab_id[i+2][p+1]][id_cel] = id_cel;
        bordure = 1;
      }
      if (bordure == 1) {
        id_tmp[i+1][p+1] = 1;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawPoint(renderer, i, p);
        bordure = 0;
      }
    }
  }

  //Enlève les ID de tab_id qui correspondent aux bordures
  for (i = 0; i < h; i++) {
    for (p = 0; p < w; p++) {
      if (id_tmp[i+1][p+1] == 1) {
        tab_id[i+1][p+1] = -1;
      }
    }
  }

  /*
  //Matrice adjacence
  j = 0;
  for (i = 0; i < nbTerritoires; i++) {
    printf("Territoire %d : ", i);
    for (j = 0; j < nbTerritoires; j++) {
      printf(" %d ", tab_voisins[i][j]);
    }
    printf("\n");
  }*/

  SCell *map_cellules = generer_cellules(tab_voisins, nbTerritoires, tab_comparaison);
  SMap *map = generer_territoire(map_cellules, nbTerritoires);

  //Render tout ce qui a été modifié au niveau graphique
  SDL_RenderPresent(renderer);

  return map;
}

/********************************************************************************************
*
* FUNCTION NAME: point_aleatoire
*
* DESCRIPTION: Génère un point aléatoire qui définit, à peu prêt, l'emplacement d'une cellule
*
* ARGUMENT      TYPE             DESCRIPTION
* h             int              Hauteur de la fenêtre de jeu
* w             int              Largeur de la fenêtre de jeu
* tab           int**            Tableau qui contient les points à générer
* row           int              Index du point généré
*
*********************************************************************************************/
void point_aleatoire(int h, int w, int **tab, int row) {
  tab[row][0] = rand()%(h);
  tab[row][1] = rand()%(w);
}

/********************************************************************************************
*
* FUNCTION NAME: couleur_aleatoire
*
* DESCRIPTION: Attribut une couleur aléatoire (parmi les 8 par défaut) à une cellule
*
* ARGUMENT         TYPE             DESCRIPTION
* tab              int**            Tableau qui contient les 8 couleurs
* row              int              Permet de savoir quelle couleur sera choisit
* couleur_actuelle int*             Permet de connaître l'index de la couleur actuelle, elle augmente et se remet à 0 une fois les 8 couleurs atteintes
* couleur          int[8][3]        Tableau qui contient les couleurs
* nbJoueurs        int              Nombre de joueurs
*
*********************************************************************************************/
void couleur_aleatoire(int **tab, int row, int *couleur_actuelle, int couleur[8][3], int nbJoueurs) {
  tab[row][0] = couleur[*couleur_actuelle][0];
  tab[row][1] = couleur[*couleur_actuelle][1];
  tab[row][2] = couleur[*couleur_actuelle][2];
  (*couleur_actuelle)++;
  if ((*couleur_actuelle) > nbJoueurs-1) {
    *couleur_actuelle = 0;
  }
}

/********************************************************************************************
*
* FUNCTION NAME: generer_cellules
*
* DESCRIPTION: Génère un tableau contenant les structures SCell de la carte
*
* ARGUMENT        TYPE             DESCRIPTION
* tab_adj         int**            Matrice d'adjacence de la carte
* nbTerritoires   int              Nombre de cellules
* tab_comparaison int*             Tableau qui permet de savoir quelle ID corresond à quel joueur
*
* RETURNS: Retourne un tableau de SCell
*
*********************************************************************************************/
SCell* generer_cellules(int **tab_adj, int nbTerritoires, int *tab_comparaison) {
  SCell *tab_cell = malloc(nbTerritoires*sizeof(SCell));

  int i, j, voisin;

  for (i = 0; i < nbTerritoires; i++) {
    tab_cell[i].id = i;
    tab_cell[i].owner = tab_comparaison[i];
    //20 voisins max (très peu probable plus)
    tab_cell[i].neighbors = malloc(20*sizeof(SCell*));
    //Implémenter le nombre de dés aléatoires
    tab_cell[i].nbDices = 3;
    voisin = 0;
    for (j = 0; j < nbTerritoires; j++) {
      if (tab_adj[i][j] != -1) {
        tab_cell[i].neighbors[voisin] = malloc(sizeof(SCell));
        tab_cell[i].neighbors[voisin] = &tab_cell[j];
        voisin++;
      }
    }
    tab_cell[i].nbNeighbors = voisin;
  }

  return tab_cell;
}

/********************************************************************************************
*
* FUNCTION NAME: generer_territoire
*
* DESCRIPTION: Génère la structure SMap du jeu
*
* ARGUMENT      TYPE             DESCRIPTION
* cellules      SCell*           Tableau qui contient toutes les cellules de la carte
* nbTerritoires int              Nombre de cellules
*
* RETURNS: Retourne la SMap générée
*
*********************************************************************************************/
SMap* generer_territoire(SCell* cellules, int nbTerritoires) {
  SMap *map;

  map->cells = cellules;
  map->nbCells = nbTerritoires;
  map->stack = 0;

  return map;
}

void lib_memoire() {

}