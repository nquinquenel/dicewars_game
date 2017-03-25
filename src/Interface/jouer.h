#include <SDL2/SDL.h>
#include "../Librairies/interface.h"

void attaquer_territoire(int cord_x, int cord_y, int h, int w, int *tab_comparaison, int** tab_id, SDL_Renderer* renderer, SMap *map, int idPlayer, int couleurs[8][3]);
int territoireSelec(int cord_x, int cord_y, int** tab_id);
void notifTerrains(int id, int** tab_id, SDL_Renderer* renderer, int h, int w, int** bord);
