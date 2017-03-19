#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Structure définissant les caractéristiques de l'ia courante
typedef struct SIa
{
	int id;	// Id de l'ia
	int nbPlayer; // Nombre de joueurs dans la partie
} SIa;

/********************************************************************************************
*
* FUNCTION NAME: CreatePlayer
*
* DESCRIPTION: création d'un nouveau joueur
*
* ARGUMENT    TYPE             DESCRIPTION
* idNewPlayer unsigned int     l'id du joueur
* name        *char            le nom du nouveau joueur
* info        *SPlayerInfo     structure qui contient le nom de la strategie et le nom des joueurs
*
* RETURNS: la structure SPlayerInfo
*
*********************************************************************************************/
SPlayerInfo* CreatePlayer(unsigned int idNewPlayer, char *name, SPlayerInfo *info);

/********************************************************************************************
*
* FUNCTION NAME: getClusterSize
*
* DESCRIPTION: renvoie la taille de la grappe de cellules
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* startingCell  *SCell           l'adresse de la cellule de départ
*
* RETURNS: la taille de la grappe de cellules
*
*********************************************************************************************/
int getClusterSize(const SMap *map, SCell *startingCell);

/********************************************************************************************
*
* FUNCTION NAME: isIntInArray
*
* DESCRIPTION: teste si un entier est dans un tableau d'entiers
*
* ARGUMENT      TYPE             DESCRIPTION
* val           int              la valeur à rechercher
* arr           *int             le tableau d'entiers
* size          int              la taille du tableau
*
* RETURNS: 1 si val est dans arr, 0 sinon
*
*********************************************************************************************/
int isValueInArray(int val, int *arr, int size);

/********************************************************************************************
*
* FUNCTION NAME: isCellInArrayOfCellPointer
*
* DESCRIPTION: teste si une adresse de cellule est dans un tableau d'adresses de cellules
*
* ARGUMENT      TYPE             DESCRIPTION
* cell          *SCell           l'adresse de la cellule à rechercher
* arrCell       **SCell          le tableau d'adresses de cellules
* size          int              la taille du tableau
*
* RETURNS: 1 si cell est dans arrCell, 0 sinon
*
*********************************************************************************************/
int isCellInArrayOfCellPointer(SCell *cell, SCell **arrCell, int size);

/********************************************************************************************
*
* FUNCTION NAME: updateStack
*
* DESCRIPTION: met à jour la pile de dés du joueur suite à l'attaque
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* attackingCell *SCell           l'adresse de la cellule attaquante
*
*********************************************************************************************/
void updateStack(const SMap *map, SCell *attackingCell);

/********************************************************************************************
*
* FUNCTION NAME: MiniSCell
*
* DESCRIPTION: fonction permettant de chercher le plus petit voisin existant
*
* ARGUMENT    TYPE             DESCRIPTION
* voisins     **SCell          Le pointeur du tableau de pointeurs
* nbVoisins   int              Le nombre de voisins de notre cellule
* RETURNS: Un tableau de 2 entiers contenant le rang et le nombre de dés
*
*********************************************************************************************/
int* MiniSCell(SCell **voisins, int nbVoisins);

/********************************************************************************************
*
* FUNCTION NAME: liberationMemoireTab
*
* DESCRIPTION: Fonction à appeler pour nettoyer un tableau de int
*
* ARGUMENT    TYPE             DESCRIPTION
* tab         *int             les éléments du tableau
* taille      int              la taille du tableau
*
*********************************************************************************************/
void liberationMemoireTab(int *tab, int taille);

/********************************************************************************************
*
* FUNCTION NAME: distributeDices
*
* DESCRIPTION: distribue les dés aléatoirement entre les cellules à la fin du tour du joueur
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
*
*********************************************************************************************/
void distributeDices(const SMap *map);
