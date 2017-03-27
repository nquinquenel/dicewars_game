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
* FUNCTION NAME: PlayTurn2
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
int PlayTurn2(const SMap *map, STurn *turn)
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

  if(coup[0] > 0) //Check si le coup est en notre défaveur (exemple : 2 dés VS 4 dés)
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
* FUNCTION NAME: PlayTurn3
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
int PlayTurn3(const SMap *map, STurn *turn)
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
      coup = MiniCoupSCell(tab[i].neighbors, tab[i].nbNeighbors, tab[i].nbDices); // attribution par défaut
      coup[0] -= tab[i].nbDices; //On décrémente par le nombre de dés que l'on a
      turn->cellFrom = tab[i].id; //attribution temporaire
      turn->cellTo = tab[i].neighbors[coup[1]]->id; //attribution temporaire
    }
    else if(MiniCoupSCell(tab[i].neighbors, tab[i].nbNeighbors, tab[i].nbDices)[0]-tab[i].nbDices < coup[0])
    {
      coup = MiniCoupSCell(tab[i].neighbors, tab[i].nbNeighbors,tab[i].nbDices); //On change stock notre adresse de tableau
      coup[0] -= tab[i].nbDices; //On doit décrémenter pour pouvoir vérifier le suivant
      turn->cellFrom = tab[i].id; //nouvelle attribution
      turn->cellTo = tab[i].neighbors[coup[1]]->id; //nouvelle attribution
    }
    if(coup[0]==1)
    {
      return 1;
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
int GetClusterSizeStrat(const SMap *map, SCell *startingCell)
{
  int i, j, k;
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
  for (k = 0; k < (map->nbCells); k++) {
    free(cellsToTest[k]);
  }
  free(cellsToTest);

  return clusterIdsSize;
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
*
* RETURNS: Un tableau de 2 entiers contenant le rang et le nombre de dés
*
*********************************************************************************************/
int* MiniSCell(SCell **voisins, int nbVoisins)
{
  int k;
  int mini; // le nombre de dés minimale
  int rang; //le rang du voisins
  int boo = 1;
  for(k = 0; k < nbVoisins; k++)
  {
    if(boo)
    {
      if(voisins[k]->owner != IA.id) // si je suis le propriétaire pour éviter d'attaquer son territoire
      {
        mini = voisins[k]->nbDices;
        rang = k;
        boo = 0;
      }
    }
    else if(voisins[k]->nbDices < mini)
    {
      if (voisins[k]->owner != IA.id ) // si je suis le propriétaire pour éviter d'attaquer son territoire
      {
        mini = voisins[k]->nbDices;
        rang = k;
      }
    }
  }
  int *tableau = malloc(2*sizeof(int));//Création du tableau de retour
  tableau[0] = mini;
  tableau[1] = rang;
  return tableau;
}

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
int* MiniCoupSCell(SCell **voisins, int nbVoisins, int nbDes)
{
  int k;
  int mini; // le nombre de dés minimale
  int rang; //le rang du voisins
  int boo = 1;
  int *tableau = malloc(2*sizeof(int));//Création du tableau de retour


  for(k = 0; k < nbVoisins; k++)
  {
    if(boo)
    {
      if(voisins[k]->owner != IA.id) // si je suis le propriétaire pour éviter d'attaquer son territoire
      {
        mini = voisins[k]->nbDices;
        rang = k;
        boo = 0;
        if(nbDes - mini == 1)
        {
          tableau[0] = mini;
          tableau[1] = rang;
          return tableau;
        }
      }
    }
    else if(voisins[k]->nbDices < mini)
    {
      if (voisins[k]->owner != IA.id ) // si je suis le propriétaire pour éviter d'attaquer son territoire
      {
        mini = voisins[k]->nbDices;
        rang = k;
        if(nbDes - mini == 1)
        {
          tableau[0] = mini;
          tableau[1] = rang;
          return tableau;
        }
      }
    }
  }
  tableau[0] = mini;
  tableau[1] = rang;
  return tableau;
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
