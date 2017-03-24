#include <SDL2/SDL.h>

int attaquer_territoire(int cord_x, int cord_y, int h, int w, int *tab_comparaison, int** tab_id, SDL_Renderer* renderer) {
  int res, i, j, id;
  id = tab_id[cord_x][cord_y];
  if (id == -1) {
    res = -1;
  } else {
    for (i = 0; i < h; i++) {
      for (j = 0; j < w; j++) {
        if (tab_id[i+1][j+1] == id) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
          SDL_RenderDrawPoint(renderer, i, j);
        }
      }
    }
    SDL_RenderPresent(renderer);
  }
  return res;
}
