/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bmb{
	int timeb;
	int x,y;
	int power;
};

struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	int power;
	struct bmb*tab[8];
};



struct player* player_init(int bomb_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bombs = bomb_number;
	player->power=2;
	int i=0;
	while(i<8){
		player->tab[i]=malloc(sizeof(struct bmb));
		player->tab[i]->timeb=0;
		i++;
	}

	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}


int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs -= 1;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

//check if we can mouv the box
	case CELL_BOX:
	switch (player->current_direction) {
		case NORTH:
			if (map_is_inside(map, x, y-1) && map_get_cell_type(map, x, y-1)==CELL_EMPTY){
				map_set_cell_type(map,x, y-1, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			}
			break;
		case SOUTH:
			if (map_is_inside(map, x, y+1) && map_get_cell_type(map, x, y+1)==CELL_EMPTY){
				map_set_cell_type(map,x, y+1, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
		}
			break;
		case WEST:
			if (map_is_inside(map, x-1, y) && map_get_cell_type(map, x-1, y)==CELL_EMPTY){
				map_set_cell_type(map,x-1, y, CELL_BOX);
				map_set_cell_type(map, x, y,CELL_EMPTY);
			return 1;
		}
			break;
		case EAST:
			if (map_is_inside(map, x+1, y) && map_get_cell_type(map, x+1, y)==CELL_EMPTY){
				map_set_cell_type(map,x+1, y, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
		}
			break;
	}
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_MONSTER:
		break;

	case CELL_BOMB:
		return 0;
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
			}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	//if (move) {
	//	map_set_cell_type(map, x, y, CELL_EMPTY);
	//}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

void player_set_bomb(struct player* player,struct map* map){
	assert(player);
	int i=0;
	while (i<8){
		if (player->tab[i]->timeb==0){
			player->tab[i]->x=player->x;
			player->tab[i]->y=player->y;
			player->tab[i]->power=player->power;
			player->tab[i]->timeb=SDL_GetTicks();
		break;
		}
		i++;
	}
}

void bomb_update(struct player* player,struct map* map){
		int i=0;
		int j=SDL_GetTicks();
		while (i<8){
			if ((player->tab[i]->timeb)!=0){
				if(j-player->tab[i]->timeb<=1000) {
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y,CELL_BOMB_4);
				}
				if((j-player->tab[i]->timeb<=2000) && (j-player->tab[i]->timeb>1000)) {
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y,CELL_BOMB_3);
				}
				if((j-player->tab[i]->timeb<=3000) && (j-player->tab[i]->timeb>2000)){
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y,CELL_BOMB_2);
				}
				if((j-player->tab[i]->timeb<=4000) && (j-player->tab[i]->timeb>3000)) {
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y,CELL_BOMB_1);
				}
			if((j-player->tab[i]->timeb<=4500) && (j-player->tab[i]->timeb>4000)){
						int power=player->tab[i]->power;
						int p=1;
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y,CELL_BOOM);
						while (p<=power){
							if (map_is_inside(map ,player->tab[i]->x+p,player->tab[i]->y)){
								if (((map_get_cell_type(map ,player->tab[i]->x+p,player->tab[i]->y))==(CELL_EMPTY)) ||((map_get_cell_type(map ,player->tab[i]->x+p,player->tab[i]->y))==(CELL_BOX))){
									map_set_cell_type(map ,player->tab[i]->x+p,player->tab[i]->y,CELL_BOOM);
						}
					}
							if (map_is_inside(map ,player->tab[i]->x-p,player->tab[i]->y)){
								if (((map_get_cell_type(map ,player->tab[i]->x-p,player->tab[i]->y))==(CELL_EMPTY)) ||((map_get_cell_type(map ,player->tab[i]->x-p,player->tab[i]->y))==(CELL_BOX))){
									map_set_cell_type(map ,player->tab[i]->x-p,player->tab[i]->y,CELL_BOOM);
						}
					}
					if (map_is_inside(map ,player->tab[i]->x,player->tab[i]->y+p)){
						if (((map_get_cell_type(map ,player->tab[i]->x,player->tab[i]->y+p))==(CELL_EMPTY)) ||((map_get_cell_type(map ,player->tab[i]->x,player->tab[i]->y+p))==(CELL_BOX))){
							map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y+p,CELL_BOOM);
						}
					}
					if (map_is_inside(map ,player->tab[i]->x,player->tab[i]->y-p)){
						if (((map_get_cell_type(map ,player->tab[i]->x,player->tab[i]->y-p))==(CELL_EMPTY)) ||((map_get_cell_type(map ,player->tab[i]->x,player->tab[i]->y-p))==(CELL_BOX))){
							map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y-p,CELL_BOOM);
						}
					}
							p++;
						}
					}
			 if(j-player->tab[i]->timeb>4500) {
					int power=player->tab[i]->power;
					int p=1;
					map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y,CELL_EMPTY);
					while (p<=power){
						if (map_is_inside(map ,player->tab[i]->x+p,player->tab[i]->y)){
							if ((map_get_cell_type(map ,player->tab[i]->x+p,player->tab[i]->y))==(CELL_BOMB)){
								map_set_cell_type(map ,player->tab[i]->x+p,player->tab[i]->y,CELL_EMPTY);
					}
				}
						if (map_is_inside(map ,player->tab[i]->x-p,player->tab[i]->y)){
							if ((map_get_cell_type(map ,player->tab[i]->x-p,player->tab[i]->y))==(CELL_BOMB)){
								map_set_cell_type(map ,player->tab[i]->x-p,player->tab[i]->y,CELL_EMPTY);
					}
				}
				if (map_is_inside(map ,player->tab[i]->x,player->tab[i]->y+p)){
					if ((map_get_cell_type(map ,player->tab[i]->x,player->tab[i]->y+p))==(CELL_BOMB)){
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y+p,CELL_EMPTY);
					}
				}
				if (map_is_inside(map ,player->tab[i]->x,player->tab[i]->y-p)){
					if ((map_get_cell_type(map ,player->tab[i]->x,player->tab[i]->y-p))==(CELL_BOMB)){
						map_set_cell_type(map ,player->tab[i]->x,player->tab[i]->y-p,CELL_EMPTY);
					}
				}
						p++;
				}
				 player->tab[i]->timeb=0;
				}
			}
			i++;
		}
}
