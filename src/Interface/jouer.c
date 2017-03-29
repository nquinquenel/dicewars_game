#include <SDL2/SDL.h>
#include "../Librairies/interface.h"

void attaquer_territoire(int cord_x, int cord_y, int h, int w, int *tab_comparaison, int** tab_id, SDL_Renderer* renderer, SMap *map, int idPlayer, int couleurs[8][3]) {
  int i, j, id;
  id = tab_id[cord_x][cord_y];
  tab_comparaison[id] = idPlayer;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      if (tab_id[i+1][j+1] == id) {
        SDL_SetRenderDrawColor(renderer, couleurs[idPlayer][0], couleurs[idPlayer][1], couleurs[idPlayer][2], 0);
        SDL_RenderDrawPoint(renderer, i, j);
      }
    }
  }
  SDL_RenderPresent(renderer);
  SCell *cell = &(map->cells[id]);
  cell->owner = idPlayer;
}

void attaquer_territoireSansCoord(int id_atq, int h, int w, int *tab_comparaison, int** tab_id, SDL_Renderer* renderer, SMap *map, int idPlayer, int couleurs[8][3]) {
  int i, j;
  tab_comparaison[id_atq] = idPlayer;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      if (tab_id[i+1][j+1] == id_atq) {
        SDL_SetRenderDrawColor(renderer, couleurs[idPlayer][0], couleurs[idPlayer][1], couleurs[idPlayer][2], 0);
        SDL_RenderDrawPoint(renderer, i, j);
      }
    }
  }
  SDL_RenderPresent(renderer);
  SCell *cell = &(map->cells[id_atq]);
  cell->owner = idPlayer;
}

int territoireSelec(int cord_x, int cord_y, int** tab_id) {
  return tab_id[cord_x][cord_y];
}

void notifTerrains(int id, int** tab_id, SDL_Renderer* renderer, int h, int w, int** tab_bord) {
  int i, p;
  for (i = 1; i < h-1; i++) {
    for (p = 1; p < w-1; p++) {
      if (tab_id[i+1][p+1] == id) {
        if (tab_id[i+1][p] == -1 || tab_id[i+1][p+2] == -1 || tab_id[i][p+1] == -1 ||
           tab_id[i+2][p+1] == -1 || tab_id[i+2][p+2] == -1 || tab_id[i+2][p] == -1 ||
           tab_id[i][p+2] == -1 || tab_id[i][p] == -1) {
          tab_bord[i][p] = 1;
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
          SDL_RenderDrawPoint(renderer, i, p);
        }
      }
    }
  }
  SDL_RenderPresent(renderer);
}
