#include <SDL2/SDL.h>

void generer_map(SDL_Renderer* renderer, int h, int w, int nbJoueurs);
void point_aleatoire(int h, int w, int **tab, int row);
void couleur_aleatoire(int **tab, int row, int *couleur_actuelle, int couleur[8][3], int nbJoueurs);
