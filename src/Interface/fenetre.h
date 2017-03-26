#include "generation.h"

/********************************************************************************************
*
* FUNCTION NAME: fenetre
*
* DESCRIPTION: Génère la fenêtre de jeu SDL2 et départ du programme (boucle du jeu)
*
*********************************************************************************************/
void fenetre();

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
void displayDices(SDL_Renderer* renderer, int pixel_x, int pixel_y, int idCell, int nbDices);
