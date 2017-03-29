#include <stdio.h>
#include <stdlib.h>
#include "Librairies/interface.h"
#include "Librairies/interfacePerso.h"
#include "Interface/fenetre.h"

int main(int argc, char **argv) {
  int i = 0;
  for (i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  int nbParties, nbPlayer;
  nbParties = argv[1][0] - '0';
  nbPlayer = argv[2][0] - '0';
  printf("test\n");
  createGame(nbParties, nbPlayer, argc, argv);

  return 0;
}
