#include "interface.h"
#include <stdlib.h>
#include <stdio.h>

// Fonction � appeler au d�but de chaque partie
// La strat�gie doit compl�ter la structure SPlayerInfo
void InitGame(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info)
{
  printf("%s\n", "dans InitGame");
}

// Fonction � appeler � chaque tour sur la strat�gie et tant que le retour de fonction est vrai et qu'il n'y a pas d'erreur.
//	* Ne pas oublier pour l'arbitre de dupliquer toute la structure map pour chaque appel !
//	* En cas d'erreur, r�tablir la carte dans l'�tat initial avant le premier tour du joueur.
// valeur de retour : bool�en : 0 coups termin�s, 1 structure turn compl�t�e avec un nouveau coup � jouer.
int PlayTurn(const SMap *map, STurn *turn)
{
  printf("%s\n", "dans PlayTurn");

  return 0;
}

// Fonction � appeler � la fin de chaque partie
void EndGame(unsigned int idWinner)
{
  printf("%s\n", "dans EndGame");
}
