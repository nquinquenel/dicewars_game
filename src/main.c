#include <stdio.h>
#include <stdlib.h>
#include "Librairies/interface.h"
#include "Librairies/interfacePerso.h"
#include "Interface/fenetre.h"
#include <dlfcn.h>

int main(int argc, char **argv) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  int nbParties, nbPlayer;
  nbParties = argv[1][0] - '0';
  nbPlayer = argv[2][0] - '0';

  int nombreIA = argc - 3;

  void **lib;

  pfInitGame *tab_InitGame;
  pfPlayTurn *tab_PlayTurn;
  pfEndGame *tab_EndGame;

  if (nombreIA > 0) {

    void **lib = malloc(nombreIA*sizeof(void*));
    tab_InitGame = malloc(nombreIA*sizeof(pfInitGame));
    tab_PlayTurn = malloc(nombreIA*sizeof(pfPlayTurn));
    tab_EndGame = malloc(nombreIA*sizeof(pfEndGame));

    for  (i = 3; i < argc; i++) {

      if ((lib[i-3] = dlopen(argv[i], RTLD_LAZY)) == NULL) {
        printf("Erreur open lib : %s\n", argv[i]);
      }

      if ((tab_InitGame[i-3] = (pfInitGame)dlsym(lib[i-3], "InitGame")) == NULL) {
        printf("Erreur init lib InitGame\n");
      }

      if ((tab_PlayTurn[i-3] = (pfPlayTurn)dlsym(lib[i-3], "PlayTurn")) == NULL) {
        printf("Erreur init lib PlayTurn\n");
      }

      if ((tab_EndGame[i-3] = (pfEndGame)dlsym(lib[i-3], "EndGame")) == NULL) {
        printf("Erreur init lib EndGame\n");
      }

    }
  }

  createGame(nbParties, nbPlayer, argc, argv, tab_InitGame, tab_PlayTurn, tab_EndGame);

  if (nombreIA > 0) {
    dlclose(lib);
    free(lib);
    free(tab_InitGame);
    free(tab_PlayTurn);
    free(tab_EndGame);
  }

  return 0;
}
