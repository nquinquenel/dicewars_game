#include "../Librairies/interface.h"
#include "../Librairies/interfacePerso.h"
#include "../Interface/fenetre.h"
SIa IA;//variable globale permettant de garder accessible les caractéristiques de l'IA courante
/********************************************************************************************
*
* FUNCTION NAME: InitGame
*
* DESCRIPTION: Fonction à appeler au début de chaque partie
*              La stratégie doit compléter la structure SPlayerInfo
*
* ARGUMENT    TYPE             DESCRIPTION
* id          unsigned int     le numero de l'ia
* nbPlayer    unsigned int     le nombre de joueurs
* info        *SPlayerInfo     structure qui contient le nom de la strategie et le nom des joueurs
*
*********************************************************************************************/
void InitGame(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info)
{
    IA.id = id; //sauvegarde de l'id de l'IA
    IA.nbPlayer = nbPlayer; //sauvegarde du nombre de joueurs
}
/********************************************************************************************
*
* FUNCTION NAME: PlayTurn
*
* DESCRIPTION: attaque avec la 1ere cellule trouvée qui à une différence de dés maximale
*              attaque si attaque autant de dés que défense
*              si on joueur possède plus de la moitié des cellules de lap, alors on n'attaque que ce joueur
*
* ARGUMENT          TYPE             DESCRIPTION
* idjoueurActuel    unsigned int
* map               const *SMap      la carte
* turn              *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn(unsigned int idjoueurActuel, const SMap *map, STurn *turn)
{
    int idFrom = -1; //id cellule attaquante
    int idTo = -1; //id cellule attaquée
    int diff = -1; //différence nbDices entre cellule attaquante et cellule attaquée
    int winningPlayer = -1; //l'id du joueur qui possede + de la moitié des cellules
    int i, j, k, m;
    SCell *territoires = map->cells; //tableau de SCell
    int nbCellsPerPlayer[IA.nbPlayer]; // tableau du nombre de cellules de chaque joueur
    for (k = 0; k < (map->nbCells); k++) //remplissage de nbCellsPerPlayer
    {
        nbCellsPerPlayer[territoires[k].owner]++;
    }

    for(m = 0; m < IA.nbPlayer; m++)
    {
        if ((PercentageOfOccupation(map, GetNbPlayerCells(map, m)) > 50) && (idjoueurActuel != m)) //si 50% de la map et pas moi
        {
            winningPlayer = m; //si le joueur a + de la moitié de la map, alors on n'attaquera que lui
            break;
        }
    }

    if (winningPlayer != -1) // si un joueur a plus de la moitié de la map
    {
        for(i = 0; i < (map->nbCells); i++) //parcours des cellules
        {
            if (territoires[i].owner == idjoueurActuel) //si le territoire appartient à l'IA
            {
                for (j = 0; j < territoires[i].nbNeighbors; j++) //parcours des voisins de ce territoire
                {
                    if((((territoires[i].neighbors[j])->owner) == winningPlayer) && (territoires[i].nbDices != 1) && (((territoires[i].nbDices) - (territoires[i].neighbors[j])->nbDices) > diff)) //si voisin = winningPlayer && si nbDices cellule attaquante > 1 && si + gde diff de dés
                    {
                        idFrom = territoires[i].id;
                        idTo = (territoires[i].neighbors[j])->id;
                        diff = ((territoires[i].nbDices) - (territoires[i].neighbors[j])->nbDices);
                    }
                }
            }
        }
    }
    else //sinon si aucun joueur ne se démarque
    {
        for(i = 0; i < (map->nbCells); i++) //parcours des cellules
        {
            if (territoires[i].owner == idjoueurActuel) //si le territoire appartient à l'IA
            {
                for (j = 0; j < territoires[i].nbNeighbors; j++) //parcours des voisins de ce territoire
                {
                    if((((territoires[i].neighbors[j])->owner) != idjoueurActuel) && (territoires[i].nbDices != 1) && (((territoires[i].nbDices) - (territoires[i].neighbors[j])->nbDices) > diff)) //si voisin = ennemi && si nbDices cellule attaquante > 1 && si + gde diff de dés
                    {
                        idFrom = territoires[i].id;
                        idTo = (territoires[i].neighbors[j])->id;
                        diff = ((territoires[i].nbDices) - (territoires[i].neighbors[j])->nbDices);
                    }
                }
            }
        }
    }

    if (diff>=0) //si on a une attaque possible
    {
        turn->cellFrom = idFrom;
        turn->cellTo = idTo;
        return 1; //on effectuera notre attaque
    }
    return 0; //on passera notre tour
}
/********************************************************************************************
*
* FUNCTION NAME: EndGame
*
* DESCRIPTION: fonction à appeler lors de la fin de partie
*
* ARGUMENT    TYPE             DESCRIPTION
* idWinner    unsigned int     l'id du joueur gagnant
*
*********************************************************************************************/
void EndGame(unsigned int id, unsigned int idWinner)
{
    //free tous les objets
    //afficher gagnant et score (le nombre de tours
    FILE *logfile = fopen("logfile.txt","a");
    char str[256] = "/tgame winner: idWinner";
    if(logfile==NULL){
        printf("cant open file");
    }
    fprintf(logfile,"%s",str);
    fclose(logfile);
}
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
int GetClusterSizeStrat(const SMap *map, SCell *startingCell)
{
    int i, j;
    int idPlayer = startingCell->owner; //l'id du joueur propriétaire de la cellule de référence
    //int clusterIds[map->nbCells]; //tableau des id des cellules de la grappe, de taille le nombre de cellules de la map, car taille maximale
    int clusterIdsSize = 0; //la taille de la grappe de cellules
    SCell **cellsToTest = malloc((map->nbCells)*sizeof(SCell *)); //tableau de pointeurs de SCell. Les cellules à tester lors du prochain while
    for (i = 0; i < (map->nbCells); i++) {
        cellsToTest[i] = malloc(sizeof(SCell));
    }
    cellsToTest[0] = startingCell; //la premiere cellule à tester dans le while est startingCell
    int cellsToTestSize = 1; //la taille du tableau de cellules à tester
    int nextCellToTest = 0; //indice de la prochaine cellule à tester
    while (cellsToTestSize != nextCellToTest) //tant qu'il reste des cellules alliées dont on n'a pas regardé les voisins
    {
        SCell *currentCell = cellsToTest[nextCellToTest]; //adresse de la cellule courante à tester
        //clusterIds[clusterIdsSize] = currentCell->id; //on rajoute l'id de la cellule courante aux cellules parcourues
        clusterIdsSize++;
        SCell **voisins = currentCell->neighbors; // Tableau de pointeur vers les cellules voisines de la cellule courante
        int nbVoisins = currentCell->nbNeighbors;
        for(j = 0; (j < nbVoisins) ; j++) //parcours des voisins
        {
            if ((voisins[j]->owner == idPlayer) && !IsCellInArrayOfCellPointerStrat(voisins[j], cellsToTest, cellsToTestSize)) //si le cellule voisine est un cellule alliée et qu'elle n'a pas déjà dans cellsToTest
            {
                cellsToTest[cellsToTestSize] = voisins[j]; //on l'a rajoute dans la liste des cellules à tester
                cellsToTestSize++;
            }
        }
        nextCellToTest++;
    }
    //libération allocation mémoire
    free(cellsToTest);
    return clusterIdsSize;
}
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
int PercentageOfOccupation(const SMap *map, int nbPlayerCells)
{
    return (int) ((nbPlayerCells*100)/map->nbCells);
}
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
int GetNbPlayerCells(const SMap *map, int idPlayer)
{
    int nbPlayerCells = 0; //le nombre de cellules du joueur ayant l'id idPlayer
    SCell *allCells = map->cells; //toutes les cellules de la map
    int i;
    for (i = 0; i < map->nbCells; i++) //si la cellule appartient au joueur
    {
        if(allCells[i].owner == idPlayer) nbPlayerCells++; //on incrémente le compteur de son nombre de cellules
    }
    return nbPlayerCells;
}
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
void LiberationMemoireTab(int *tab, int taille)
{
    int i;
    for(i = 0; i < taille; i++)
    {
        //free(tab[i]);
    }
    free(tab);
}
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
int IsIntInArray(int val, int *arr, int size)
{
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val) return 1;
    }
    return 0;
}
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
int IsCellInArrayOfCellPointerStrat(SCell *cell, SCell **arrCell, int size)
{
    int i;
    for (i=0; i < size; i++) {
        if (arrCell[i] == cell) return 1;
    }
    return 0;
}
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
int AreNeighborsStrat(SCell *cell1, SCell *cell2)
{
    int i;
    for (i = 0; i < (cell1->nbNeighbors); i++) {
        if (cell2 == (cell1->neighbors[i])) //si les 2 cellules sont voisines
        {
            return 1;
        }
    }
    return 0;
}
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
SCluster* getCluster(SCluster **clusters, int clustersSize, SCell *cell)
{
    int i;
    for (i = 0; i < clustersSize; i++) //parcours des clusters
    {
        if (clusters[i] != NULL) //si le cluster n'est pas nul suite à la fusion de 2 clusters
        {
            if(IsCellInArrayOfCellPointerStrat(cell, clusters[i]->cells, clusters[i]->nbCells)) return clusters[i]; //si la cellules est dans le SCluster
        }
    }
    return NULL; //la cellule n'est dans aucun SCluster
}
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
SCluster* createCluster(SCell *cell, int clustersSize)
{
    SCluster *newCluster_p = malloc(sizeof(SCluster *)); //allocation mémoire pour le SCluster
    newCluster_p[0].id = clustersSize;
    newCluster_p[0].nbCells = 1;
    newCluster_p[0].cells = malloc(sizeof(SCell *)); //allocation mémoire pour le tableau de pointeurs de SCell
    newCluster_p[0].cells[0] = cell;
    return newCluster_p;
}
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
SCell* GetCell(const SMap *map, int idCell)
{
    int i;
    SCell *allCells = map->cells; //tableau de toutes les SCell de la map
    for (i = 0; i < (map->nbCells); i++)
    {
        if (allCells[i].id == idCell) {
            return &(allCells[i]);
        }
    }
    return NULL;
}
