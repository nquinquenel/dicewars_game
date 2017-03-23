#define SDL_main main
#include <stdio.h>
#include <stdlib.h>
#include "Librairies/interface.h"
#include "Librairies/interfacePerso.h"
#include "Interface/fenetre.h"

int main() {
  InitGame(1, 8, NULL);
  fenetre();

  return 0;
}
