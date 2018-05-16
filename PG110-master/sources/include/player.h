/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct box_bonus;
struct player;
struct bmb;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Give, Increase, Decrease,set the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_set_nb_bomb(struct player*player,int i);

// Give, Increase, Decrease,set life
int player_get_life(struct player*player);
void player_inc_life(struct player * player);
void player_dec_life(struct player * player);
void player_set_life(struct player*player,int i);

//Increase,Decrease,return,set the range of bombs
void player_inc_r_bomb(struct player*player);
void player_dec_r_bomb(struct player*player);
int player_get_r_bomb(struct player*player);
void player_set_r_bomb(struct player*player,int i);

//Give,increase,Decrease,set keys number
void inc_nb_key(struct player* player);
void dec_nb_key(struct player* player);
int player_get_key(struct player*player);
void player_set_nb_key(struct player*player,int i);

//Return current level
int player_cur_level(struct player*player);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

//Set a bomb at the player's location
void player_set_bomb(struct player* player,struct map*map);
void chain_explo(int x,int y,int timer,struct player* player);
void bomb_update(struct player*player,struct map* map);

void bonus_apparition(struct player*player,struct map*map);

//invulnerability frame
void invulnerability(struct player*player,struct map*map);

#endif
