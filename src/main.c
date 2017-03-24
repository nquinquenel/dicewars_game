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

  return 0;
}
