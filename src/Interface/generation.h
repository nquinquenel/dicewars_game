#include <SDL2/SDL.h>
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
SMap* generer_map(SDL_Renderer* renderer, int h, int w, int nbJoueurs, int nbTerritoires, int *tab_comparaison, int** tab_id);

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
void point_aleatoire(int h, int w, int **tab, int row);

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
void couleur_aleatoire(int **tab, int row, int *couleur_actuelle, int couleur[8][3], int nbJoueurs);

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
SCell* generer_cellules(int **tab_adj, int nbTerritoires, int *tab_comparaison);

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
SMap* generer_territoire(SCell* cellules, int nbTerritoires);

void lib_memoire();
