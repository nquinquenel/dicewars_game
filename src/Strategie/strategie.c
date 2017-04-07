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
* FUNCTION NAME: PlayTurn1
*
* DESCRIPTION: attaque avec la 1ere cellule trouvée qui à une différence de dés maximale
*              attaque si attaque autant de dés que défense
*
* ARGUMENT          TYPE             DESCRIPTION
* idjoueurActuel    unsigned int
* map               const *SMap      la carte
* turn              *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn1(unsigned int idjoueurActuel, const SMap *map, STurn *turn)
{
    int idFrom = -1; //id cellule attaquante
    int idTo = -1; //id cellule attaquée
    int diff = -1; //différence nbDices entre cellule attaquante et cellule attaquée
    int i, j;
    SCell *territoires = map->cells; //tableau de SCell

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
* FUNCTION NAME: PlayTurn2
*
* DESCRIPTION: attaque avec la 1ere cellule trouvée qui à une différence de dés maximale
*              n'attaque pas si attaque autant de dés que défense
*
* ARGUMENT          TYPE             DESCRIPTION
* idjoueurActuel    unsigned int
* map               const *SMap      la carte
* turn              *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn2(unsigned int idjoueurActuel, const SMap *map, STurn *turn)
{
    int idFrom = -1; //id cellule attaquante
    int idTo = -1; //id cellule attaquée
    int diff = 0; //différence nbDices entre cellule attaquante et cellule attaquée
    int i, j;
    SCell *territoires = map->cells; //tableau de SCell

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

    if (diff>=1) //si on a une attaque possible
    {
        turn->cellFrom = idFrom;
        turn->cellTo = idTo;
        return 1; //on effectuera notre attaque
    }
    return 0; //on passera notre tour
}

/********************************************************************************************
*
* FUNCTION NAME: PlayTurn3
*
* DESCRIPTION: attaque avec la cellule qui a le plus de dés et qui à la plus petite différence de dés avec la cellule attaquée
*
* ARGUMENT          TYPE             DESCRIPTION
* idjoueurActuel    unsigned int
* map               const *SMap      la carte
* turn              *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn3(unsigned int idjoueurActuel, const SMap *map, STurn *turn)
{
    int idFrom = -1; //id cellule attaquante
    int idTo = -1; //id cellule attaquée
    int diff = -1; //différence nbDices entre cellule attaquante et cellule attaquée
    int i, j;
    SCell *territoires = map->cells; //tableau de SCell

    for(i = 0; i < (map->nbCells); i++) //parcours des cellules
    {
        if (territoires[i].owner == idjoueurActuel) //si le territoire appartient à l'IA
        {
            for (j = 0; j < territoires[i].nbNeighbors; j++) //parcours des voisins de ce territoire
            {
                if ((((territoires[i].neighbors[j])->owner) != idjoueurActuel) && ( (territoires[i].nbDices > 1) && ((territoires[i].nbDices) >= ((territoires[i].neighbors[j])->nbDices)) ))
                {
                    if (diff == -1) //si pas encore d'attaque définie
                    {
                        idFrom = territoires[i].id;
                        idTo = (territoires[i].neighbors[j])->id;
                        diff = ((territoires[i].nbDices) - (territoires[i].neighbors[j])->nbDices);
                    }
                    else if((((territoires[i].nbDices) - ((territoires[i].neighbors[j])->nbDices)) == diff) && ((GetCell(map, idFrom)->nbDices) < (territoires[i].nbDices))) //si attaque définie && la diff est la meme que l'existante && la nouvelle cellule attaquante a plus de dés que la cellule attaquante actuelle
                    {
                        idFrom = territoires[i].id;
                        idTo = (territoires[i].neighbors[j])->id;
                        diff = ((territoires[i].nbDices) - (territoires[i].neighbors[j])->nbDices);
                    }
                    else if(((territoires[i].nbDices) - ((territoires[i].neighbors[j])->nbDices)) < diff) //si attaque définie && la diff est est plus petite que l'actuelle
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
* FUNCTION NAME: PlayTurn4
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
int PlayTurn4(unsigned int idjoueurActuel, const SMap *map, STurn *turn)
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
* FUNCTION NAME: PlayTurn5
*
* DESCRIPTION: attaque la cellule selon un chemin sûr pour se rapprocher / atteindre une grappe de cellules alliées.
*              et ainsi former une grosse grappe de cellules pour avoir plus de dés distribués à la fin du tour
*
* ARGUMENT          TYPE             DESCRIPTION
* idjoueurActuel    unsigned int
* map               const *SMap      la carte
* turn              *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn5(unsigned int idjoueurActuel, const SMap *map, STurn *turn)
{
    int i, k, m, o;
    int j = 0;
    int myCellsSize =  GetNbPlayerCells(map, idjoueurActuel); //mon nombre de cellules
    SCell ** myCells = (SCell**)malloc(myCellsSize*sizeof(SCell *)); //tableau de pointeurs vers les cellules alliées
    int clustersSize = 0; //le nombre de SCluster dans clusters
    SCluster **clusters; //tableau de pointeurs vers des structures SCluster
    for (i = 0; (i < map->nbCells); i++) //remplissage de myCellsSize
    {
        if (((map->cells[i]).owner) == idjoueurActuel) //si la cellule m'appartient
        {
            myCells[j] = GetCell(map, (map->cells[i]).id);
            j++;
        }
    }

    printf("myCellsSize = %d\n", myCellsSize);

    for (k = 0; k < myCellsSize; k++) //parcours des cellules alliées pour construire les structures SCluster -> 1ère cellule
    {
        for (m = 0; m < myCellsSize; m++) //parcours des cellules alliées pour construire les structures SCluster -> 2ème cellule
        {
            printf("\t\tclustersSize = %d\n", clustersSize);
            printf(" k = %d\n", k);
            printf(" m = %d\n", m);
            if (clustersSize) //si il y a des SCluster
            {
                printf("%s\n", "avant AreNeighborsStrat");
                if (AreNeighborsStrat(myCells[k], myCells[m])) //si les 2 cellules sont voisines directement (i.e. sont adjacentes)
                {
                    printf("%s\n", "avant getCluster c1");
                    SCluster *c1 = (SCluster*)getCluster(clusters, clustersSize, myCells[k]); //récupération de l'adresse du SCluster de la cellule 1
                    printf("%s\n", "avant getCluster c2");
                    SCluster *c2 = (SCluster*)getCluster(clusters, clustersSize, myCells[m]); //récupération de l'adresse du SCluster de la cellule 2

                    if (c1 == NULL) //si c1 n'est pas dans un cluster
                    {
                        printf("%s\n", "(c1 == NULL)");
                        if (c2 == NULL) //c2 non plus
                        {
                            printf("\t%s\n", "(c2 == NULL)");
                            clusters = (SCluster**)realloc(clusters, (clustersSize+1)*sizeof(SCluster *)); //ré-allocation mémoire pour le tableau de pointeurs de SCluster
                            clusters[clustersSize] = (SCluster*)createCluster(myCells[k], clustersSize); //on créé un cluster pour la 1ère cellule
                            clustersSize++;

                            //on rajoute la 2ème cellule au SCluster de la 1ère cellule
                            clusters[c1->id]->cells = (SCell**)realloc(clusters[c1->id]->cells, (clusters[c1->id]->nbCells+1)*sizeof(SCell *)); //ré-allocation mémoire avec la nouvelle taille
                            clusters[c1->id]->cells[clusters[c1->id]->nbCells]= myCells[m];
                            (clusters[c1->id]->nbCells)++;

                        }
                        else //c2 dans un cluster
                        {
                            printf("\t%s\n", "(c2 != NULL)");

                            //on rajoute la 1ère cellule au SCluster de la 2ère cellule
                            clusters[c2->id]->cells = (SCell**)realloc(clusters[c2->id]->cells, (clusters[c2->id]->nbCells+1)*sizeof(SCell *)); //ré-allocation mémoire avec la nouvelle taille
                            clusters[c2->id]->cells[clusters[c2->id]->nbCells]= myCells[k];
                            (clusters[c2->id]->nbCells)++;
                        }
                    }
                    else //sinon c1 a un cluster
                    {
                        printf("%s\n", "(c1 != NULL)");

                        if (c2 == NULL) //si c2 n'a pas de cluster
                        {
                            printf("\t%s\n", "(c2 == NULL)");
                            //on rajoute la 2ème cellule au SCluster de la 1ère cellule
                            clusters[c1->id]->cells = (SCell**)realloc(clusters[c1->id]->cells, (clusters[c1->id]->nbCells+1)*sizeof(SCell *)); //ré-allocation mémoire avec la nouvelle taille
                            clusters[c1->id]->cells[clusters[c1->id]->nbCells]= myCells[m];
                            (clusters[c1->id]->nbCells)++;
                        }
                        else if((c2->id) != (c1->id)) //sinon si les 2 cellules sont dans un cluster différent
                        {
                            printf("\t%s\n", "(c2 != NULL)");
                            int tailleClusterc1 = clusters[c1->id]->nbCells;
                            int tailleClusterc2 = clusters[c2->id]->nbCells;
                            if (tailleClusterc1 >= tailleClusterc2) //si on fusionne c2 dans c1
                            {
                                printf("\t%s\n", "On met c2 dans c1 ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !");
                                printf("\t((clusters[c2->id])->nbCells) = %d\n", ((clusters[c2->id])->nbCells));
                                int a;

                                printf("c2->id = %d\n", c2->id);
                                printf("c1->id = %d\n", c1->id);
                                // on rajoute le cluster de la cellule 2 dans celui de la cellule 1
                                for (a = 0; a < ((clusters[c2->id])->nbCells); a++) //parcours du tableau de pointeurs de cellules du cluster de la cellule 2
                                {
                                    if (a <5) {
                                        printf("\t Dans if a <5 -> ((clusters[c2->id])->nbCells) = %d\n", ((clusters[c2->id])->nbCells));
                                        printf("\t Dans if a <5 -> ((clusters[c1->id])->nbCells) = %d\n", ((clusters[c1->id])->nbCells));
                                    }
                                    ((clusters[c1->id])->cells) = (SCell**)realloc(((clusters[c1->id])->cells), (((clusters[c1->id])->nbCells)+1)*sizeof(SCell *));
                                    if (a <5) {
                                        printf("\t Dans if a <5 -> ((clusters[c2->id])->nbCells) = %d\n", ((clusters[c2->id])->nbCells));
                                        printf("\t Dans if a <5 -> ((clusters[c1->id])->nbCells) = %d\n", ((clusters[c1->id])->nbCells));
                                    } //ré-allocation mémoire pour le 2ème cluster
                                    ((clusters[c1->id])->cells)[((clusters[c1->id])->nbCells)] = ((clusters[c2->id])->cells[a]);
                                    if (a <5) {
                                        printf("\t Dans if a <5 -> ((clusters[c2->id])->nbCells) = %d\n", ((clusters[c2->id])->nbCells));
                                        printf("\t Dans if a <5 -> ((clusters[c1->id])->nbCells) = %d\n", ((clusters[c1->id])->nbCells));
                                    }
                                    (clusters[c1->id]->nbCells)++;
                                    if (a <5) {
                                        printf("\t Dans if a <5 -> ((clusters[c2->id])->nbCells) = %d\n", ((clusters[c2->id])->nbCells));
                                        printf("\t Dans if a <5 -> ((clusters[c1->id])->nbCells) = %d\n", ((clusters[c1->id])->nbCells));
                                    }
                                }
                                printf("\t%s\n", "avant free");
                                free((clusters[c2->id])->cells); //libération mémoire du tableau de pointeurs de cellule du cluster de la 2ème cellule
                                clusters[c2->id] = NULL;
                            }
                            else //sinon si on fusionne c1 dans c2
                            {
                                printf("\t%s\n", "On met c1 dans c2 ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !");
                                printf("((clusters[c1->id])->nbCells) = %d\n", ((clusters[c1->id])->nbCells));
                                int a;
                                // on rajoute le cluster de la cellule 1 dans celui de la cellule 2
                                for (a = 0; a < ((clusters[c1->id])->nbCells); a++) //parcours du tableau de pointeurs de cellules du cluster de la cellule 1
                                {
                                    ((clusters[c2->id])->cells) = (SCell**)realloc(((clusters[c2->id])->cells), (((clusters[c2->id])->nbCells)+1)*sizeof(SCell *)); //ré-allocation mémoire pour le 2ème cluster
                                    ((clusters[c2->id])->cells)[((clusters[c2->id])->nbCells)] = ((clusters[c1->id])->cells[a]);
                                    (clusters[c2->id]->nbCells)++;
                                }
                                printf("\t%s\n", "avant free");
                                free((clusters[c1->id])->cells); //libération mémoire du tableau de pointeurs de cellule du cluster de la 1ère cellule
                                clusters[c1->id] = NULL;
                            }
                        }
                    }
                }
                else if((myCells[k]->id) == (myCells[m]->id)) //si on parle de la même cellule
                {
                    printf("%s\n", "--------------------------------------");
                    printf("%s\n", "si on parle de la meme cellule");
                    printf("%s\n", "--------------------------------------");
                    SCluster *c1 = (SCluster*)getCluster(clusters, clustersSize, myCells[k]);

                    if (c1 == NULL) //si la cellule n'est pas déjà dans un SCluster
                    {
                        printf("\t%s\n", "if (c1 == NULL)");
                        clusters = (SCluster**)realloc(clusters, (clustersSize+1)*sizeof(SCluster *)); //ré-allocation mémoire pour le tableau de pointeurs de SCluster
                        printf("\t%s\n", "apres realloc");
                        clusters[clustersSize] = (SCluster*)createCluster(myCells[k], clustersSize); //on créé un cluster pour la cellule
                        clustersSize ++;
                    }
                    else
                    {
                        // rien a fiare ici
                        printf("\t%s\n", "if (c1 != NULL) (rien à faire)");
                    }
                }
            }
            else //sinon si pas encore de SCluster d'initialisé (qd on rentre ici, 1 seule fois, les 2 cellules ont la même adresse)
            {
                printf("%s\n", "si clusters vide");
                clusters = (SCluster**)malloc(sizeof(SCluster *)); //allocation mémoire pour le tableau de pointeurs de SCluster
                clusters[clustersSize] = (SCluster*)createCluster(myCells[k], clustersSize); //on créé un cluster pour la 1ère cellule
                clustersSize ++;
            }
        }
    }

    printf("%s\n", "avant liberation allocation memoire");
    // libération allocation mémoire
    for (o = 0; o < clustersSize; o++) //parcours des SCluster
    {
        if (clusters[o] != NULL) {
            printf("%s\n", "1er free");
            free(clusters[o]->cells); //free tableau de pointeurs de SCell
        }
        printf("%s\n", "2eme free");
        free(clusters[o]); //free de chaque SCluster
    }
    printf("%s\n", "free clusters");
    free(clusters); //free tableau de pointeurs de SCluster

    return 0;
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
        else{
            printf("%s\n", "cluster NULL dans getCluster");
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
