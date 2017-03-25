#include <SDL2/SDL.h>
#include "../Librairies/interface.h"

SMap* generer_map(SDL_Renderer* renderer, int h, int w, int nbJoueurs, int nbTerritoires, int *tab_comparaison, int** tab_id);
void point_aleatoire(int h, int w, int **tab, int row);
void couleur_aleatoire(int **tab, int row, int *couleur_actuelle, int couleur[8][3], int nbJoueurs);
SCell* generer_cellules(int **tab_adj, int nbTerritoires, int *tab_comparaison);
SMap* generer_territoire(SCell* cellules, int nbTerritoires);
void lib_memoire();
