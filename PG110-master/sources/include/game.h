/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new or a saved game
struct game* game_new();
struct game*game_load(int lev,int x,int y,int nbmb,int rbmb,int nkey,int life);

// Free a game
void game_free(struct game* game);

//Return,Set the current level
int game_get_current_level(struct game* game);
void set_game_level(struct game*game,int i);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map, map's level i
struct map* game_get_current_map(struct game* game);
struct map* game_get_map(struct game* game,int i);

// Display the game on the screen
void game_display(struct game* game);

// Update
void update_map(struct game*game);
void update_window(struct game*game);
int game_update(struct game* game);

//Save the game
void player_save_game(struct game * game);


#endif /* GAME_H_ */
