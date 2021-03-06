#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "interface.h"

// taille du buffer pour l'écriture dans le fichier de log
#define INT_BUFFER_SIZE 10

typedef void (*pfInitGame)(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info);
typedef int (*pfPlayTurn)(unsigned int idjoueurActuel, const SMap *map, STurn *turn);
typedef void (*pfEndGame)(unsigned int id, unsigned int idWinner);

// Structure définissant les caractéristiques de l'ia courante
typedef struct SIa
{
	int id;	// Id de l'ia
	int nbPlayer; // Nombre de joueurs dans la partie
} SIa;

// Structure définissant les caractéristiques de l'ia courante
typedef struct SContext
{
	int id;	// Id du joueur
	int nbPlayer; // Nombre de joueurs dans la partie
	int highestCluster; // la taille de la plus grosse grappe de cellules alliées
} SContext;

// Structure définissant une grappe de cellules alliées voisines
typedef struct SCluster
{
	int id;	// Id du cluster
	int nbCells; // nombre de cellules dans le cluster
	struct SCell **cells; // Tableau de pointeurs vers les cellules du cluster
} SCluster;

void createGame(int nbParties, int nbPlayer, int nbArg, char** noms, pfInitGame* tab_InitGame, pfPlayTurn* tab_PlayTurn, pfEndGame* tab_EndGame);
int demandeAttaque(SMap *map, STurn *turn, int idPlayer);
int getIdJoueurActuel();
int getNbIA();
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
int isAnIA(int id);
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
* FUNCTION NAME: GetClusterSizeStrat
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
int GetClusterSizeStrat(const SMap *map, SCell *startingCell);
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
* FUNCTION NAME: IsCellInArrayOfCellPointerStrat
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
int IsCellInArrayOfCellPointerStrat(SCell *cell, SCell **arrCell, int size);
/********************************************************************************************
*
* FUNCTION NAME: UpdateHighestCluster
*
* DESCRIPTION: met à jour highestCluster du SContext du joueur
*              on ne fera rien dans le cas d'une attaque ratée, car pas de modification au niveau des propriétaires des territoires
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* startingCell  *SCell           l'adresse de la cellule qui a attaquante si idjoueur = joueur attaquant
*                                NULL si idjoueur = joueur attaqué
* idPlayer      int              l'id du joueur pour lequel il faut mettre à jour la pile de dés après l'attaque / défense
*
*********************************************************************************************/
void UpdateHighestCluster(const SMap *map, SCell *startingCell, int idPlayer);
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
* FUNCTION NAME: AreNeighborsStrat
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
int AreNeighborsStrat(SCell *cell1, SCell *cell2);
/********************************************************************************************
*
* FUNCTION NAME: GetContexts
*
* DESCRIPTION: renvoie contexts, le tableau de pointeur de struct SContext
*
* RETURNS: le tableau de pointeur de struct SContext
*
*********************************************************************************************/
SContext** GetContexts();
/********************************************************************************************
*
* FUNCTION NAME: PlayTurn
*
* DESCRIPTION: la startegie
*
* ARGUMENT          TYPE             DESCRIPTION
* idjoueurActuel    unsigned int
* map               const *SMap      la carte
* turn              *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn(unsigned int idjoueurActuel, const SMap *map, STurn *turn);
/********************************************************************************************
*
* FUNCTION NAME: getCluster
*
* DESCRIPTION: teste une cellule est dans un SCluster
*
* ARGUMENT      TYPE             DESCRIPTION
* clusters      **SCluster       le tableau de pointeurs vers des structures SCluster
* cell          *SCell           l'adresse de la cellule
*
* RETURNS: l'adresse du SCluster si la cellule est dedans, parmis le tableau de pointeurs
*          NULL sinon
*
*********************************************************************************************/
SCluster* getCluster(SCluster **clusters, int clustersSize, SCell *cell);
/********************************************************************************************
*
* FUNCTION NAME: createCluster
*
* DESCRIPTION: créer un nouveau SCluster ciontenant une cellule
*
* ARGUMENT      TYPE             DESCRIPTION
* cell          *SCell           l'adresse de la cellule
* clusterSize   int              la taille actuelle du tableau de SCluster pour déterminer l'id du cluster à créer
*
* RETURNS: l'adresse du nouveau SCluster créé
*
*********************************************************************************************/
SCluster* createCluster(SCell* cell, int clustersSize);
char* concat( char *s1,  char *s2);
void writetoLog(char *s);
char* concatint(char *s1, int n);
