#include "../Librairies/interface.h"
#include "../Librairies/interfacePerso.h"
int idJoueurActuel;
int* idIA;
int nbIA;
SContext **contexts; // tableau de pointeurs de SContext
/********************************************************************************************
*
* FUNCTION NAME: createGame //SUJET A DES CHANGEMENTS AU NIVEAU DES PARAMETRES
*
* DESCRIPTION: Initialise et démarre le jeu
*
* ARGUMENT    TYPE             DESCRIPTION
* nbParties   int              le nombre de parties à jouer
* nbPlayer    int              le nombre de joueurs dans la partie
* nbArg       int              nombres d'arguments (nbParties + nbPlayer + les IA)
* noms        char**           le nom des IA
*
* RETURNS: L'id du joueur actuel
*
*********************************************************************************************/
void createGame(int nbParties, int nbPlayer, int nbArg, char** noms, pfInitGame* tab_InitGame, pfPlayTurn1* tab_PlayTurn1, pfEndGame* tab_EndGame) {
    int i, j, temp;
    contexts = malloc(nbPlayer*sizeof(SContext *)); //tableau de pointeurs de SContext

    srand ( time(NULL) );

    int tabAleaJoueurs[nbPlayer];
    for (i = 0; i < nbPlayer; i++) {
      tabAleaJoueurs[i] = i;
    }
    for (i = nbPlayer-1; i > 0; i--) {
        j = rand() % (i+1);

        temp = tabAleaJoueurs[i];
        tabAleaJoueurs[i] = tabAleaJoueurs[j];
        tabAleaJoueurs[j] = temp;
    }

    for (i = 0; i < nbPlayer; i++) {
        contexts[i] = malloc(sizeof(SContext));
        contexts[i]->id = tabAleaJoueurs[i]; //id du joueur
        contexts[i]->highestCluster = 0; //taille de la plus grosse grappe de cellules alliées. 0 à l'initialisation
        contexts[i]->nbPlayer = nbPlayer; //nb de joueurs dans la partie
    }

    nbIA = nbArg - 3;
    idIA = malloc(nbIA * sizeof(int));
    printf("Nombre d'IA : %d\n", nbIA);
    SPlayerInfo **sp = malloc(nbIA*sizeof(SPlayerInfo*));
    for (j = 0; j < nbIA; j++) {
        sp[j] = malloc(sizeof(SPlayerInfo));
        tab_InitGame[j](tabAleaJoueurs[j+(nbPlayer-nbIA)], nbPlayer, sp[j]);
        idIA[j] = tabAleaJoueurs[j+(nbPlayer-nbIA)];
    }
    idJoueurActuel = 0;
    fenetre(nbPlayer, nbParties, tab_InitGame, tab_PlayTurn1, tab_EndGame);
}

int isAnIA(int id) {
    int i = 0;
    for (i = 0; i < nbIA; i++) {
        if (id == idIA[i]) {
            return 1;
        }
    }
    return 0;
}
/********************************************************************************************
*
* FUNCTION NAME: demandeAttaque
*
* DESCRIPTION: Fait une demande d'attaque, si elle est correcte alors l'attaque se fait
*
* ARGUMENT    TYPE             DESCRIPTION
* map         SMap*            la map actuelle
* turn        STurn*           le tour à jouer
* idPlayer    int              l'id du joueur qui attaque
*
* RETURNS: 1 si l'attaque a réussi, 0 si non, et -1 si la demande d'attaque n'est pas valide
*
*********************************************************************************************/
int demandeAttaque(SMap *map, STurn *turn, int idPlayer) {
    if (ValidTurn(map, turn, idPlayer) == 1) {
        return Attack(map, turn);
    } else {
        return -1;
    }
}
/********************************************************************************************
*
* FUNCTION NAME: getIdJoueurActuel
*
* DESCRIPTION: Retourne l'id du joueur actuel
*
* ARGUMENT    TYPE             DESCRIPTION
*
* RETURNS: L'id du joueur actuel
*
*********************************************************************************************/
int getIdJoueurActuel() {
    return idJoueurActuel;
}

int getNbIA() {
  return nbIA;
}

/********************************************************************************************
*
* FUNCTION NAME: setIdJoueurActuel
*
* DESCRIPTION: Permet de changer l'id du joueur actuel (à travers un autre fichier par exemple)
*
* ARGUMENT    TYPE             DESCRIPTION
* id          int              la nouvelle id
* nbJoueurs   int              le nombre de joueurs dans la partie (afin d'avoir un idJoueurActuel toujours correct)
*
*********************************************************************************************/
void setIdJoueurActuel(int id, int nbJoueurs) {
    if (id > nbJoueurs-1) {
        id = 0;
    }
    idJoueurActuel = id;
}
/********************************************************************************************
*
* FUNCTION NAME: CreatePlayer
*
* DESCRIPTION: création d'un nouveau joueur dans la struct SPlayerInfo
*
* ARGUMENT    TYPE             DESCRIPTION
* idNewPlayer unsigned int     l'id du joueur
* name        *char            le nom du nouveau joueur
* info        *SPlayerInfo     structure qui contient le nom de la strategie et le nom des joueurs
*
* RETURNS: la structure SPlayerInfo
*
*********************************************************************************************/
SPlayerInfo* CreatePlayer(unsigned int idNewPlayer, char *name, SPlayerInfo *info)
{
    //MAJ du nom du joueur
    snprintf(info -> members[idNewPlayer], sizeof(info -> members[idNewPlayer]), "%s", name); //copie des <arg2> 1ers char de <arg4> dans <arg1>, en rajoutant '\0'
    return info;
}
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
void UpdateHighestCluster(const SMap *map, SCell *startingCell, int idPlayer)
{
    int clusterSize = 0; //la taille de la plus grosse grappe
    int i;
    if (startingCell == NULL) //si idPlayer est le joueur qui a été attaqué
    {
        SCell *allCells = map->cells; // toutes les cellules de la map
        for (i = 0; i < (map->nbCells); i++) //parcours des cellules de la map pour récupérer la taille de la plus grosse grappe de cellules
        {
            if(allCells[i].owner == idPlayer) //si idPlayer est le propriétaire de la cellule
            {
                int tmp = GetClusterSize(map, &allCells[i]);
                if (tmp > clusterSize) clusterSize = tmp;
            }
        }
        contexts[idPlayer]->highestCluster = clusterSize;
    }
    else //si idPlayer est le joueur qui a attaqué
    {
        clusterSize = GetClusterSize(map, startingCell); //récupération de la taille de la grappe
        if ((contexts[idPlayer]->highestCluster) < clusterSize) contexts[idPlayer]->highestCluster = clusterSize; //si le territoire s'est agrandi suite à l'attaque
    }
}
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
void DistributeDices(const SMap *map)
{
    int i, j;
    SCell *allCells = map->cells; //tableau de toutes les SCell de la map
    SCell **myCells = malloc((map->nbCells)*sizeof(SCell *)); //tableau de pointeurs de SCell. Les cellules à tester lors du prochain while
    for (i = 0; i < (map->nbCells); i++) {
        myCells[i] = malloc(sizeof(SCell));
    }
    int myCellsSize = 0; //nb de cellules qui sera = à map->nbCells (donc SCell NULL comprises)
    int onlyMyCellsSize= 0; //nb de cellules qui sera = aux cellules != NULL dans myCells
    for (j = 0; j < map->nbCells; j++) //remplissage de myCells
    {
        if ((idJoueurActuel == allCells[j].owner) && (allCells[j].nbDices<8)) //si la cellule m'appartient et qu'elle a moins de 8 dés
        {
            myCells[myCellsSize] = &(allCells[j]);
            onlyMyCellsSize++;
        }
        else
        {
            myCells[myCellsSize] = NULL;
        }
        myCellsSize++;
    }
    int nbDicesToDistribute = (map->stack[idJoueurActuel]) + contexts[idJoueurActuel]->highestCluster; // le nb de dés à distribuer = la taille de la pile + la taille de la plus grosse grappe
    printf("nb de dés à distribuer = %d\n", nbDicesToDistribute);
    srand(time(NULL)); // initialisation de rand
    int randCell; //place de la cellule dans myCells tirée aléatoirement
    while (nbDicesToDistribute && onlyMyCellsSize) //tant qu'il reste des dés à distribuer et qu'il reste des cellules qui ont moins de 8 dés
    {
        randCell = rand()%myCellsSize;
        if (myCells[randCell] != NULL)
        {
            myCells[randCell]->nbDices++; //incrémentation du nombre de dés de la cellule
            nbDicesToDistribute--; //décrémentation du nb de dés restant à distribuer
            if ((myCells[randCell]->nbDices) == 8) //si la cellule a maintenant 8 dés
            {
                myCells[randCell] = NULL; //on la supprime de myCells pour ne pas augmenter le nombre de dés à nouveau
                onlyMyCellsSize--;
            }
        }
    }
    //maj de la stack du joueur courant apres la distribution des dés
    if (nbDicesToDistribute<40) map->stack[idJoueurActuel] = nbDicesToDistribute;
    else map->stack[idJoueurActuel] = 40; //si plus de 40 dés restant à distribuer

    free(myCells);
}
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
int ValidTurn(const SMap *map, STurn *turn, int idPlayer)
{
    SCell *attackingCell = GetCell(map, turn->cellFrom); //adresse de la cellule attaquante
    SCell *defendingCell = GetCell(map, turn->cellTo);  //adresse de la cellule défendante
    //teste si les cellules sont bien voisines
    if (!AreNeighbors(attackingCell, defendingCell)) return 0; //si les cellules ne sont pas voisines
    if(attackingCell->nbDices == 1) return 0; //si la cellule attaquante n'a qu'un dés
    if(attackingCell->owner != idPlayer) return 0; //si la cellule attaquante n'appartient pas au joueur courant
    if(defendingCell->owner == idPlayer) return 0; //si la cellule attaquée appartient au joueur courant
    return 1;
}
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
int Attack(const SMap *map, STurn *turn)
{
    srand(time(NULL)); // initialisation de rand
    SCell *attackingCell = GetCell(map, turn->cellFrom); //adresse de la cellule attaquante
    SCell *defendingCell = GetCell(map, turn->cellTo);  //adresse de la cellule défendante
    int i, j;
    //on lance les dés de la cellule attaquante
    int attackScore = 0;
    for (i = 0; i < attackingCell->nbDices; i++)
    {
        attackScore += (rand()%5)+1;
    }
    //on lance les dés de la cellule attaquée
    int defenseScore = 0;
    for (j = 0; j < defendingCell->nbDices; j++)
    {
        defenseScore += (rand()%5)+1;
    }
    printf("%d VS %d", attackScore, defenseScore);
    if (attackScore>defenseScore) //si l'attaque gagne
    {
        printf("%s\n", "  -> gagné");
        defendingCell->owner = attackingCell->owner;
        defendingCell->nbDices = (attackingCell->nbDices)-1;
        attackingCell->nbDices = 1;
        return 1; //on a gagné l'attaque
    }
    printf("%s\n", "  -> perdu");
    attackingCell->nbDices = 1; //le nombre de dés de la cellule attaquante descendent à 1
    return 0; //on a perdu l'attaque
}
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
int AreNeighbors(SCell *cell1, SCell *cell2)
{
    int i;
    for (i = 0; i < cell1->nbNeighbors; i++) {
        if (cell2 == cell1->neighbors[i]) //si les 2 cellules sont voisines
        {
            return 1;
        }
    }
    return 0;
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
int GetClusterSize(const SMap *map, SCell *startingCell)
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
            if ((voisins[j]->owner == idPlayer) && !IsCellInArrayOfCellPointer(voisins[j], cellsToTest, cellsToTestSize)) //si le cellule voisine est un cellule alliée et qu'elle n'a pas déjà dans cellsToTest
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
int IsCellInArrayOfCellPointer(SCell *cell, SCell **arrCell, int size)
{
    int i;
    for (i=0; i < size; i++) {
        if (arrCell[i] == cell) return 1;
    }
    return 0;
}

/********************************************************************************************
*
* FUNCTION NAME: GetContexts
*
* DESCRIPTION: renvoie contexts, le tableau de pointeur de struct SContext
*
* RETURNS: le tableau de pointeur de struct SContext
*
*********************************************************************************************/
SContext** GetContexts(){
    return contexts;
}
