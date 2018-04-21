/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_


enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70 ,	   		// 	0111 0000
	CELL_BOOM=0x80,

};
enum monster_direction{
	MONSTER_S=0,
	MONSTER_N=1,
	MONSTER_W=2,
	MONSTER_E=3,
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC=2,
	BONUS_BOMB_NB_DEC=3,
	BONUS_BOMB_NB_INC=4,
	BONUS_MONSTER=5,
	BONUS_LIFE=6,
};

enum bomb_time{
	BOMB_4=0,
	BOMB_3=1,
	BOMB_2=2,
	BOMB_1=3,
};

enum scenery_type {
	SCENERY_STONE=0,     // 0000
	SCENERY_TREE=1,      // 0010
	SCENERY_PRINCESS=2   // 0010
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

  CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
  CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_DEC,
  CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_INC,
  CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,
  CELL_BOX_MONSTER  = CELL_BOX | BONUS_MONSTER,

	CELL_BOMB_4 = CELL_BOMB | BOMB_4,
	CELL_BOMB_3 = CELL_BOMB | BOMB_3,
	CELL_BOMB_2 = CELL_BOMB | BOMB_2,
	CELL_BOMB_1 = CELL_BOMB | BOMB_1,

	CELL_MS = CELL_MONSTER | MONSTER_S,
	CELL_MN = CELL_MONSTER | MONSTER_N,
	CELL_MW = CELL_MONSTER | MONSTER_W,
	CELL_ME = CELL_MONSTER | MONSTER_E,

	BONUS_N_INC = CELL_BONUS | BONUS_BOMB_NB_INC,
	BONUS_N_DEC = CELL_BONUS | BONUS_BOMB_NB_DEC,
	BONUS_R_INC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
	BONUS_R_DEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	BONUS_L     = CELL_BONUS | BONUS_LIFE,

};

struct map;
struct mst;

// Create a new empty map
struct map* map_new(int width, int height);

void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
enum compose_type map_get_cell_type2(struct map*map,int x,int y);


// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display bombs on the screen
void display_bombs(struct map* map, int x, int  y, unsigned char type);

// Display monsters on the screen
void display_monster(struct map* map, int x, int  y, unsigned char type);

// Display the map on the screen
void map_display(struct map* map);

// Update monster positions
void update_monster(struct map*map);
void monster_reini(struct map*map,int x, int y);
void monster_add(struct map*map,int x, int y);

// Apply bonus to the player
int player_bonus(struct map*map, int x,int y);

#endif /* MAP_H_ */
