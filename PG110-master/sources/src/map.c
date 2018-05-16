/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>


struct map {
	int width;
	int height;
	unsigned char* grid;
	struct mst*tab[80];     //monsters
};

struct mst{						//monster pos
	int x,y;
	int timem;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}
int k=0;
	while (k<=50){
		map->tab[k]=malloc(sizeof(struct mst));
		map->tab[k]->timem=0;
		k++;
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

enum compose_type map_get_cell_type3(struct map*map,int x){
	return map->grid[x];
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if ((map->width>x)&&(x>=0)&&(map->height>y)&&(y>=0))
		return 1;
	return 0;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum compose_type map_get_cell_type2(struct map*map,int x,int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_LIFE:
	 window_display_image(sprite_get_bonus(5), x, y);
	 break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;

	case SCENERY_PRINCESS:
	  window_display_image(sprite_get_princess(), x, y);
		break;
	}
}

void display_bombs(struct map* map, int x, int  y, unsigned char type){
	switch (type & 0x0f) {
		case BOMB_4:
			window_display_image(sprite_get_bombs(BOMB_4), x, y);
		break;

		case BOMB_3	:
			window_display_image(sprite_get_bombs(BOMB_3), x, y);
		break;

		case BOMB_2:
			window_display_image(sprite_get_bombs(BOMB_2), x, y);
		break;

		case BOMB_1:
			window_display_image(sprite_get_bombs(BOMB_1), x, y);
		break;
	}
}

void display_monster(struct map* map, int x, int  y, unsigned char type){
	switch (type & 0x0f) {
		case MONSTER_S:
		 window_display_image(sprite_get_monster(MONSTER_S),x,y);
		break;
		case MONSTER_N:
		 window_display_image(sprite_get_monster(MONSTER_N),x,y);
		break;
		case MONSTER_W:
		 window_display_image(sprite_get_monster(MONSTER_W),x,y);
		break;
		case MONSTER_E:
		 window_display_image(sprite_get_monster(MONSTER_E),x,y);
		break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	  case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	  case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	  case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	  case CELL_DOOR:
	      window_display_image(sprite_get_door_opened(),x,y);
	      break;
		case CELL_DOOR_CLOSED:
				window_display_image(sprite_get_door_closed(),x,y);
				break;
		case CELL_BOMB:
			display_bombs(map, x, y, type);
			break;
		case CELL_BOOM:
			window_display_image(sprite_get_boom(),x,y);
			break;
		case CELL_MONSTER:
			display_monster(map,x,y,type);
			break;
	    }
	  }
	}
}

struct map* map_get_static(int z,int i)
{

	FILE * level =NULL;
	if (i==0){
	char carte[14];
	sprintf(carte,"map/map_%d.txt",z);
		level=fopen(carte,"r");

}
if (i==1){
	char carte2[21];
	sprintf(carte2,"data/map_saved_%d.txt",z);
		level=fopen(carte2,"r");
}

	int WIDTH=0;
	int HEIGHT=0;
	fscanf(level,"%d",&WIDTH);
	fseek(level,2,SEEK_CUR);
	fscanf(level,"%d",&HEIGHT);

	struct map* map = map_new(WIDTH,HEIGHT);

		int cell=0;
	for (int i = 0; i < WIDTH * HEIGHT; i++){
		fscanf(level,"%d",&cell);
		map->grid[i] = cell;
	}
fclose(level);

		//monsters initialization
		int k=0;
	for(int l=0; l< HEIGHT;l++){
		for(int m=0; m< WIDTH;m++){
			if((map->grid[CELL(l,m)] & 11110000)==CELL_MONSTER){
					map->tab[k]->x=l;
					map->tab[k]->y=m;
					map->tab[k]->timem=SDL_GetTicks();
					k++;
				}
			}
		}
	return map;
}




void monster_update(struct map*map){
	int k=0;
  int j=SDL_GetTicks();
  while (k<=50){
		int n=0;
		int s=0;
		int e=0;
		int w=0;
		int z=0;
		if (j-map->tab[k]->timem >1000 && map->tab[k]->timem!=0){
			if(map_is_inside(map,map->tab[k]->x+1,map->tab[k]->y) && (map_get_cell_type(map,map->tab[k]->x+1,map->tab[k]->y)==CELL_EMPTY || map_get_cell_type(map,map->tab[k]->x+1,map->tab[k]->y)==CELL_BONUS)) {
				  e++;
			  }
				if(map_is_inside(map,map->tab[k]->x-1,map->tab[k]->y) && (map_get_cell_type(map,map->tab[k]->x-1,map->tab[k]->y)==CELL_EMPTY || map_get_cell_type(map,map->tab[k]->x-1,map->tab[k]->y)==CELL_BONUS)) {
					w++;
				}
				if(map_is_inside(map,map->tab[k]->x,map->tab[k]->y+1) && (map_get_cell_type(map,map->tab[k]->x,map->tab[k]->y+1)==CELL_EMPTY || map_get_cell_type(map,map->tab[k]->x,map->tab[k]->y+1)==CELL_BONUS)) {
					s++;
				}
				if(map_is_inside(map,map->tab[k]->x,map->tab[k]->y-1) && (map_get_cell_type(map,map->tab[k]->x,map->tab[k]->y-1)==CELL_EMPTY || map_get_cell_type(map,map->tab[k]->x,map->tab[k]->y-1)==CELL_BONUS)) {
					n++;
				}
				while(z==0){
					int p=rand()%4;
					if (p==0 && n==1){
						map_set_cell_type(map,map->tab[k]->x,map->tab[k]->y,CELL_EMPTY);
						map_set_cell_type(map,map->tab[k]->x,map->tab[k]->y-1,CELL_MN);
						map->tab[k]->y--;
						z++;
					}
					else if (p==1 && s==1){
						map_set_cell_type(map,map->tab[k]->x,map->tab[k]->y,CELL_EMPTY);
						map_set_cell_type(map,map->tab[k]->x,map->tab[k]->y+1,CELL_MS);
						map->tab[k]->y++;
						z++;
					}
					else if (p==2 && w==1){
						map_set_cell_type(map,map->tab[k]->x,map->tab[k]->y,CELL_EMPTY);
						map_set_cell_type(map,map->tab[k]->x-1,map->tab[k]->y,CELL_MW);
						map->tab[k]->x--;
						z++;
					}
					else if (p==3 && e==1){
						map_set_cell_type(map,map->tab[k]->x,map->tab[k]->y,CELL_EMPTY);
						map_set_cell_type(map,map->tab[k]->x+1,map->tab[k]->y,CELL_ME);
						map->tab[k]->x++;
						z++;
					}
					else if (n==0 && s==0 && e==0 && w==0){
						z++;
					}
				}
				map->tab[k]->timem=j;
		}
		k++;
	}
}

void monster_reini(struct map*map,int x,int y){
	int k=0;
	while (k<50){
		if (map->tab[k]->x==x && map->tab[k]->y == y){
			map->tab[k]->timem=0;
		}
		k++;
}
}


int player_bonus(struct map*map,int x,int y){
	switch (map->grid[CELL(x,y)] & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		return 1;
	case BONUS_BOMB_RANGE_DEC:
		return 2;
	case BONUS_BOMB_NB_DEC:
		return 3;
	case BONUS_BOMB_NB_INC:
		return 4;
	case BONUS_MONSTER:
		return 5;
	case BONUS_LIFE:
		return 6;
	case BONUS_KEY:
		return 7;
	}
	return 0;
}

void monster_add(struct map*map,int x, int y){
	int k=0;
	while (k<50){
		if (map->tab[k]->timem==0){
			map->tab[k]->timem=SDL_GetTicks();
			map->tab[k]->x=x;
			map->tab[k]->y=y;
			break;
		}
		k++;
	}
}
