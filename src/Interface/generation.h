#include <SDL2/SDL.h>
#include "../Librairies/interface.h"

SMap* generer_map(SDL_Renderer* renderer, int h, int w, int nbJoueurs, int nbTerritoires, int *tab_comparaison, int** tab_id, int** tab_points);
void point_aleatoire(int h, int w, int **tab, int row);
void couleur_aleatoire(int **tab, int row, int *couleur_actuelle, int couleur[8][3], int nbJoueurs);
SCell* generer_cellules(int **tab_adj, int nbTerritoires, int *tab_comparaison, SDL_Renderer* renderer, int** tab_points, int nbJoueurs);
/********************************************************************************************
*
* FUNCTION NAME: generer_territoire
*
* DESCRIPTION: Génère la structure SMap du jeu
*
* ARGUMENT      TYPE             DESCRIPTION
* cellules      SCell*           Tableau qui contient toutes les cellules de la carte
* nbTerritoires int              Nombre de cellules
* nbJoueurs     int              Nombre de joueurs
*
* RETURNS: Retourne la SMap générée
*
*********************************************************************************************/
SMap* generer_territoire(SCell* cellules, int nbTerritoires, int nbJoueurs);
void lib_memoire();
int compare( const void* a, const void* b);
