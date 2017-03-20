#include "../Librairies/interface.h"
#include "../Librairies/interfacePerso.h"

SIa IA;//varibale globale permettant de garder accessible les caractéristiques de l'IA courante

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
* DESCRIPTION: Fonction à appeler à chaque tour sur la stratégie et tant que le retour de
*              fonction est vrai et qu'il n'y a pas d'erreur.
*              - Ne pas oublier pour l'arbitre de dupliquer toute la structure map pour chaque appel !
*              - En cas d'erreur, rétablir la carte dans l'état initial avant le premier tour du joueur.
*
* ARGUMENT    TYPE             DESCRIPTION
* map         const *SMap      la carte
* turn        *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn(const SMap *map, STurn *turn)
{
  int nbTerritoires = map->nbCells;
  SCell *territoires = map->cells; //tableau de SCell

  // Premiere étape : récupération du nombre de territoires nous appartenant
  int i;
  int count = 0;
  for(i = 0; i < nbTerritoires; i++) //parcours des cellules
  {
    if (territoires[i].owner == IA.id ) //si je suis le propriétaire
    {
      count++; // On incrémente pour chaque territoire nous appartenant
    }
  }

  // Deuxieme étape : attribution des cellules de notre territoire dans un tableau
  SCell *tab = malloc((count)*sizeof(SCell)); // On malloc notre tableau
  count = 0; // On réinitialise le compteur
  for(i = 0; i < nbTerritoires; i++) // parcours des cellules
  {
    if (territoires[i].owner == IA.id ) // si je suis le propriétaire
    {
      tab[count] = territoires[i]; // On ajoute chaque cellule à notre tableau
      count++; // On incrémente le compteur
    }
  }

  // Troisieme étape : on cherche le territoire avec la plus grosse probabilité de victoires
  int* coup; //Contiendra l'adresse du résultat du test des voisins
  for(i = 0; i < count+1; i++) //parcours des cellules
  {
    if(i == 0)
    {
      coup = MiniSCell(tab[i].neighbors, tab[i].nbNeighbors); // attribution par défaut
      coup[0] -= tab[i].nbDices; //On décrémente par le nombre de dés que l'on a
      turn->cellFrom = tab[i].id; //attribution temporaire
      turn->cellTo = tab[i].neighbors[coup[1]]->id; //attribution temporaire
    }
    else if(MiniSCell(tab[i].neighbors, tab[i].nbNeighbors)[0]-tab[i].nbDices < coup[0]) //On vérifie en enlevant directement le nb de dés si c'est inférieur
    {
      coup = MiniSCell(tab[i].neighbors, tab[i].nbNeighbors); //On change stock notre adresse de tableau
      coup[0] -= tab[i].nbDices; //On doit décrémenter pour pouvoir vérifier le suivant
      turn->cellFrom = tab[i].id; //nouvelle attribution
      turn->cellTo = tab[i].neighbors[coup[1]]->id; //nouvelle attribution
    }
  }

  if(coup[0] >= 0) //Check si le coup est en notre défaveur (exemple : 2 dés VS 4 dés)
  {
    return 0; //on passera notre tour
  }

  //libération de l'allocation mémoire
  free(tab);
  free(coup);

  return 1; //on effectuera notre attaque
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
void EndGame(unsigned int idWinner)
{
  //free tous les objets
  //afficher gagnant et score (le nombre de tours)
}

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
SPlayerInfo* CreatePlayer(unsigned int idNewPlayer, char *name, SPlayerInfo *info)
{
  //MAJ du nom du joueur
  snprintf(info -> members[idNewPlayer], sizeof(info -> members[idNewPlayer]), "%s", name); //copie des <arg2> 1ers char de <arg4> dans <arg1>, en rajoutant '\0'

  return info;
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
  int i, j, k;
  int idPlayer = startingCell->owner; //l'id du joueur propriétaire de la cellule de référence

  int clusterIds[map->nbCells]; //tableau des id des cellules de la grappe, de taille le nombre de cellules de la map, car taille maximale
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
    clusterIds[clusterIdsSize] = currentCell->id; //on rajoute l'id de la cellule courante aux cellules parcourues
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
  for (k = 0; k < (map->nbCells); k++) {
    free(cellsToTest[k]);
  }
  free(cellsToTest);

  return clusterIdsSize;
}

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
void UpdateStack(const SMap *map, SCell *startingCell, int attacking, int success, int idPlayer)
{
  int clusterSize = 0; //la taille de la plus grosse grappe
  int i;

  if (attacking && success) //si startingCell est la cellule qui a attaqué et que l'attaque a réussi
  {
    clusterSize = GetClusterSize(map, startingCell); //récupération de la taille de la grappe

    if ((map->stack[idPlayer]) < clusterSize) //si le nombre de dés a augmenté suite à l'attaque
    {
      if (clusterSize>40) map->stack[idPlayer] = 40; //si taille > 40, on n'en met que 40, la taille maximum d'après les règles du jeu
      else map->stack[idPlayer] = clusterSize;
    }
  }

  // sinon si startingCell est la cellule qui a attaqué et que l'attaque a échoué
  // OU si startingCell est la cellule qui a été attaquée et que l'attaque a réussi
  else if ((attacking && !success) || (!attacking && success))
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
    if (clusterSize>40) map->stack[idPlayer] = 40; //si taille > 40, on n'en met que 40, la taille maximum d'après les règles du jeu
    else map->stack[idPlayer] = clusterSize;
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
  int myCellsSize = 0; //nb de cellules

  for (j = 0; j < map->nbCells; j++) //remplissage de myCells
  {
    if ((IA.id == allCells[i].owner) && (allCells[i].nbDices<8)) //si la cellule m'appartient et qu'elle a moins de 8 dés
    {
      myCells[myCellsSize] = &(allCells[i]);
      myCellsSize++;
    }
    else
    {
      myCells[myCellsSize] = NULL;
      myCellsSize++;
    }
  }

  int nbDices = map->stack[IA.id]; // le nb de dés à distribuer
  srand(time(NULL)); // initialisation de rand
  int randCell; //place de la cellule dans myCells tirée aléatoirement
  while (nbDices && myCellsSize) //tant qu'il reste des dés à distribuer et qu'il reste des cellules qui ont moins de 8 dés
  {
    randCell = rand()%myCellsSize;
    if (myCells[randCell] != NULL)
    {
      myCells[randCell]->nbDices++; //incrémentation du nombre de dés de la cellule
      if ((myCells[randCell]->nbDices) == 8) //si la cellule a 8 dés
      {
        myCells[randCell] = NULL; //on la supprime de myCells pour ne pas augmenter le nombre de dés à nouveau
        myCellsSize--;
      }
    }
  }
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

  if(attackingCell->nbDices == 1) return 0; //si la cellule attaquante n'a qu'un dés
  if(attackingCell->owner != idPlayer) return 0; //si la cellule attaquante n'appartient pas au joueur courant
  if(defendingCell->owner == idPlayer) return 0; //si la cellule attaquée appartient au joueur courant
  return 1;
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

  for (i = 0; i <( map->nbCells); i++)
  {
    if (allCells[i].id == idCell) {
      return &(allCells[i]);
    }
  }

  return NULL;
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

  if (attackScore>defenseScore) //si l'attaque gagne
  {
    defendingCell->owner = attackingCell->owner;
    defendingCell->nbDices = (attackingCell->nbDices)-1;
    attackingCell->nbDices = 1;

    return 1; //on a gagné l'attaque
  }

  attackingCell->nbDices = 1; //le nombre de dés de la cellule attaquante descendent à 1
  return 0; //on a perdu l'attaque
}

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
int* MiniSCell(SCell **voisins, int nbVoisins)
{
  int k;
  int mini; // le nombre de dés minimale
  int rang; //le rang du voisins

  for(k = 0; k < nbVoisins; k++)
  {
    if(k == 0)
    {
      mini = voisins[k]->nbDices;
      rang = k;
    }
    else if(voisins[k]->nbDices < mini)
    {
      mini = voisins[k]->nbDices;
      rang = k;
    }
  }
  int *tableau = malloc(2*sizeof(int)); //Création du tableau de retour
  tableau[0] = mini;
  tableau[1] = rang;
  return tableau;
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

//LANCEUR
int main(int argc, char const *argv[])
{
  printf("%s\n", " -> dans main de strategie");

  return 0;
}
