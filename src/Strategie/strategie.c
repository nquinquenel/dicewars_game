#include "../Librairies/interface.h"
#include "../Librairies/interfacePerso.h"
#include <stdlib.h>
#include <stdio.h>

// Fonction � appeler au d�but de chaque partie
// La strat�gie doit compl�ter la structure SPlayerInfo
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
  return 1;
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
* id          unsigned int     l'id du joueur
* name        *char            le nom du nouveau joueur
* info        *SPlayerInfo     structure qui contient le nom de la strategie et le nom des joueurs
*
* RETURNS: la structure SPlayerInfo
*
*********************************************************************************************/
SPlayerInfo* CreatePlayer(unsigned int idNewPlayer, char *name, SPlayerInfo *info)
{
  return NULL;
}


//LANCEUR
int main(int argc, char const *argv[]) {
  printf("%s\n", "dans main de strategie");
  return 0;
}
