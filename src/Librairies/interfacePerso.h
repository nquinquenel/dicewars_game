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

void createGame(int nbParties, int nbPlayer, int nbArg, char** noms);
int demandeAttaque(SMap *map, STurn *turn, int idPlayer);
int getIdJoueurActuel();
void setIdJoueurActuel(int id, int nbJoueurs);

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
* FUNCTION NAME: GetClusterSize
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
int GetClusterSize(const SMap *map, SCell *startingCell);
/********************************************************************************************
*
* FUNCTION NAME: IsIntInArray
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
int IsValueInArray(int val, int *arr, int size);
/********************************************************************************************
*
* FUNCTION NAME: IsCellInArrayOfCellPointer
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
int IsCellInArrayOfCellPointer(SCell *cell, SCell **arrCell, int size);
/********************************************************************************************
*
* FUNCTION NAME: UpdateStack
*
* DESCRIPTION: met à jour la pile de dés du joueur suite à une attaque / défense
*              on ne fera rien si startingCell est la cellule qui a été attaquée et que l'attaque a échoué
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* startingCell  *SCell           l'adresse de la cellule attaquante / attaquée
* attacking     int              1 si startingCell est la cellule qui a attaqué, 0 si c'est la cellule qui a été attaquée
* success       int              1 si l'attaque a réussi, 0 sinon
* idPlayer      int              l'id du joueur pour lequel il faut mettre à jour la pile de dés après l'attaque / défense
*
*********************************************************************************************/
void UpdateStack(const SMap *map, SCell *startingCell, int attacking, int success, int idPlayer);
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
* FUNCTION NAME: LiberationMemoireTab
*
* DESCRIPTION: Fonction à appeler pour nettoyer un tableau de int
*
* ARGUMENT    TYPE             DESCRIPTION
* tab         *int             les éléments du tableau
* taille      int              la taille du tableau
*
*********************************************************************************************/
void LiberationMemoireTab(int *tab, int taille);
/********************************************************************************************
*
* FUNCTION NAME: DistributeDices
*
* DESCRIPTION: distribue les dés aléatoirement entre les cellules à la fin du tour du joueur
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
*
*********************************************************************************************/
void DistributeDices(const SMap *map);
/********************************************************************************************
*
* FUNCTION NAME: GetCell
*
* DESCRIPTION: renvoie l'adresse de la SCell qui a l'id idCell
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* idCell        int              l'id de la cellule à retourner
*
* RETURNS: l'adresse de la cellule qui a l'id idCell, NULL sinon
*
*********************************************************************************************/
SCell* GetCell(const SMap *map, int idCell);
/********************************************************************************************
*
* FUNCTION NAME: Attack
*
* DESCRIPTION: attaque en fonction du STurn
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* turn          *STurn           le tour courant
*
* RETURNS: 1 si on a gagné l'attaque, 0 sinon
*
*********************************************************************************************/
int Attack(const SMap *map, STurn *turn);
/********************************************************************************************
*
* FUNCTION NAME: ValidTurn
*
* DESCRIPTION: teste si le coup est valide
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* turn          *STurn           le tour courant
* idPlayer      int              l'id du joueur qui effectue le tour
*
* RETURNS: 1 si le coup est valide, 0 sinon
*
*********************************************************************************************/
int ValidTurn(const SMap *map, STurn *turn, int idPlayer);
/********************************************************************************************
*
* FUNCTION NAME: PercentageOfOccupation
*
* DESCRIPTION: renvoie le pourcentage d'occupation de la map du joueur
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* nbPlayerCells int              le nombre de cellules du joueur
*
* RETURNS: un entier entre 0 et 100
*
*********************************************************************************************/
int PercentageOfOccupation(const SMap *map, int nbPlayerCells);
/********************************************************************************************
*
* FUNCTION NAME: GetNbPlayerCells
*
* DESCRIPTION: renvoie le nombre de cellules appartenant au joueur
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* idPlayer      int              l'id du joueur courant
*
* RETURNS: e nombre de cellules appartenant au joueur
*
*********************************************************************************************/
int GetNbPlayerCells(const SMap *map, int idPlayer);

/********************************************************************************************
*
* FUNCTION NAME: AreNeighbors
*
* DESCRIPTION: teste si deux cellules sont voisines
*
* ARGUMENT      TYPE             DESCRIPTION
* cell1          *SCell          l'adresse de la cellule n°1
* cell1          *SCell          l'adresse de la cellule n°2
*
* RETURNS: 1 si les cellules sont voisines, 0 sinon
*
*********************************************************************************************/
int AreNeighbors(SCell *cell1, SCell *cell2);
/********************************************************************************************
*
* FUNCTION NAME: MiniCoupSCell
*
* DESCRIPTION: fonction permettant de chercher un voisin qui a une diffférence de 1 avec notre cellule
*
* ARGUMENT    TYPE             DESCRIPTION
* voisins     **SCell          Le pointeur du tableau de pointeurs
* nbVoisins   int              Le nombre de voisins de notre cellule
* nbDes       int              Le nombre de dés de notre cellule
*
* RETURNS: Un tableau de 2 entiers contenant le rang et le nombre de dés
*
*********************************************************************************************/
int* MiniCoupSCell(SCell **voisins, int nbVoisins, int nbDes);
