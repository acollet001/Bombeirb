/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_getenv.h>


#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <player.h>
int main(int argc, char * argv[]) {

  SDL_Surface * menu;
  SDL_Rect posClick;
  SDL_Event event;
  int cont = 1;
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    error("Can't init SDL:  %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }


  SDL_WM_SetIcon(IMG_Load("sprite/icon.png"), NULL);
  window_create(WIN_MENU_L, WIN_MENU_l);
  menu = IMG_Load("sprite/menu.png");
  window_display_image(menu, 0, 0);
  window_refresh();
  while (cont) {
    SDL_PollEvent( & event);
    switch (event.type) {
    case SDL_QUIT:
      cont = 0;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        cont = 0;
        break;
        default:
          break;
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if (event.button.button == SDL_BUTTON_LEFT) {
        posClick.x = event.button.x;
        posClick.y = event.button.y;

        if (event.button.x > 290 && event.button.x < 390 && event.button.y > 360 && event.button.y < 400) {

          struct game * game = game_new();
          int i = 0;
          i = game_get_current_level(game);
          char carte[14];
          sprintf(carte, "map/map_%d.txt", i);
          FILE * level = NULL;
          level = fopen(carte, "r");
          int WIDTH = 0;
          int HEIGHT = 0;
          fscanf(level, "%d", & WIDTH);
          fseek(level, 2, SEEK_CUR);
          fscanf(level, "%d", & HEIGHT);
          fclose(level);
          window_create(SIZE_BLOC * WIDTH,
            SIZE_BLOC * HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

          SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

          // to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
          int ideal_speed = 1000 / DEFAULT_GAME_FPS;
          int timer, execution_speed;

          // game loop
          // static time rate implementation

          int done = 0;
          while (!done) {
            timer = SDL_GetTicks();

            done = game_update(game);
            game_display(game);

            execution_speed = SDL_GetTicks() - timer;
            if (execution_speed < ideal_speed)
              SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
          }

          game_free(game);

          SDL_Quit();

          return EXIT_SUCCESS;

        }
        if (event.button.x > 290 && event.button.x < 390 && event.button.y > 410 && event.button.y < 440) {
          FILE * save = NULL;
          save = fopen("data/save.txt", "r");
          int worlds, lev, x, y, nbmb, rbmb, nkey, life;
          fscanf(save, "%d", & worlds);
          fscanf(save, "%d", & lev);
          fseek(save, 1, SEEK_CUR);
          fscanf(save, "%d", & x);
          fseek(save, 1, SEEK_CUR);
          fscanf(save, "%d", & y);
          fscanf(save, "%d", & nbmb);
          fseek(save, 2, SEEK_CUR);
          fscanf(save, "%d", & rbmb);
          fseek(save, 2, SEEK_CUR);
          fscanf(save, "%d", & nkey);
          fseek(save, 2, SEEK_CUR);
          fscanf(save, "%d", & life);
          fclose(save);
          struct game * gamel = game_load(lev, x, y, nbmb, rbmb, nkey, life);
          int i = 0;
          i = game_get_current_level(gamel);
          char carte[21];
          sprintf(carte, "data/map_saved_%d.txt", i);
          FILE * level = NULL;
          level = fopen(carte, "r");
          int WIDTH = 0;
          int HEIGHT = 0;
          fscanf(level, "%d", & WIDTH);
          fseek(level, 2, SEEK_CUR);
          fscanf(level, "%d", & HEIGHT);
          fclose(level);
          window_create(SIZE_BLOC * WIDTH,
            SIZE_BLOC * HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

          SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

          // to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
          int ideal_speed = 1000 / DEFAULT_GAME_FPS;
          int timer, execution_speed;

          // game loop
          // static time rate implementation

          int done = 0;
          while (!done) {
            timer = SDL_GetTicks();

            done = game_update(gamel);
            game_display(gamel);

            execution_speed = SDL_GetTicks() - timer;
            if (execution_speed < ideal_speed)
              SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
          }

          game_free(gamel);

          SDL_Quit();

          return EXIT_SUCCESS;

        }
      }
      break;
    }
  }

}
