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

struct box_bonus
{
  int x, y;
  int bonus;
  int timebon;
};

struct bmb
{
  int timeb;
  int x, y;
  int power;
  struct box_bonus *box[4];	// for the 4 cardinal directions
};

struct player
{
  int timep;			// invulnerability frame
  int x, y;
  enum direction current_direction;
  int nb_bombs;
  int life;
  int key;
  int power;
  struct bmb *tab[20];
  int cur_lev;
};



struct player *
player_init (int bomb_number)
{
  struct player *player = malloc (sizeof (*player));
  if (!player)
    error ("Memory error");

  player->current_direction = SOUTH;
  player->nb_bombs = bomb_number;
  player->power = 1;
  player->timep = 0;
  player->key = 0;
  player->cur_lev = 0;
  int i = 0;
  int j = 0;
  player->life = 2;
  while (i < 20)
    {
      player->tab[i] = malloc (sizeof (struct bmb));
      player->tab[i]->timeb = 0;
      while (j < 4)
	{
	  player->tab[i]->box[j] = malloc (sizeof (struct box_bonus));
	  player->tab[i]->box[j]->bonus = 0;
	  player->tab[i]->box[j]->timebon = 0;
	  j++;
	}
      i++;
    }

  return player;
}



void
player_set_position (struct player *player, int x, int y)
{
  assert (player);
  player->x = x;
  player->y = y;
}


void
player_free (struct player *player)
{
  assert (player);
  free (player);
}

int
player_get_x (struct player *player)
{
  assert (player != NULL);
  return player->x;
}

int
player_get_y (struct player *player)
{
  assert (player != NULL);
  return player->y;
}

void
player_set_current_way (struct player *player, enum direction way)
{
  assert (player);
  player->current_direction = way;
}

void
player_set_nb_bomb (struct player *player, int i)
{
  assert (player);
  player->nb_bombs = i;
}

int
player_get_nb_bomb (struct player *player)
{
  assert (player);
  return player->nb_bombs;
}

void
player_inc_nb_bomb (struct player *player)
{
  assert (player);
  player->nb_bombs += 1;
}

void
player_dec_nb_bomb (struct player *player)
{
  assert (player);
  if (player->nb_bombs > 1)
    player->nb_bombs -= 1;
}

void
player_inc_r_bomb (struct player *player)
{
  assert (player);
  player->power++;
}

void
player_dec_r_bomb (struct player *player)
{
  assert (player);
  if (player->power > 1)
    player->power--;
}

int
player_get_r_bomb (struct player *player)
{
  assert (player);
  return player->power;
}

void
player_set_r_bomb (struct player *player, int i)
{
  assert (player);
  player->power = i;
}

int
player_get_life (struct player *player)
{
  assert (player);
  return player->life;
}

void
player_inc_life (struct player *player)
{
  assert (player);
  player->life++;
}

void
player_dec_life (struct player *player)
{
  assert (player);
  if (player->life > 1)
    {
      player->life--;
    }
  else
    {
      SDL_Surface *go;
      SDL_Event event;
      int cont = 1;
      window_create (WIN_GO_L, WIN_GO_l);
      go = IMG_Load ("sprite/gameover.jpg");
      window_display_image (go, 0, 0);
      window_refresh ();
      while (cont)
	{
	  SDL_PollEvent (&event);
	  switch (event.type)
	    {
	    case SDL_QUIT:
	      cont = 0;
	      break;
	    case SDL_KEYDOWN:
	      switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		  cont = 0;
		  break;
		default:
		  break;
		}
	    }
	}
    }
}

void
player_set_life (struct player *player, int i)
{
  assert (player);
  player->life = i;
}

void
inc_nb_key (struct player *player)
{
  assert (player);
  player->key++;
}

void
dec_nb_key (struct player *player)
{
  assert (player);
  player->key--;
}

int
player_get_key (struct player *player)
{
  assert (player);
  return player->key;
}

void
player_set_nb_key (struct player *player, int i)
{
  assert (player);
  player->key = i;
}

int
player_cur_level (struct player *player)
{
  assert (player);
  return player->cur_lev;
}


static int
player_move_aux (struct player *player, struct map *map, int x, int y)
{

  if (!map_is_inside (map, x, y))
    return 0;

  switch (map_get_cell_type (map, x, y))
    {
    case CELL_SCENERY:
      if (map_get_cell_type2 (map, x, y) == CELL_PRINCESS)
	{
	  SDL_Surface *vic;
	  SDL_Event event;
	  int cont = 1;
	  window_create (WIN_VIC_L, WIN_VIC_l);
	  vic = IMG_Load ("sprite/Victory.png");
	  window_display_image (vic, 0, 0);
	  window_refresh ();
	  while (cont)
	    {
	      SDL_PollEvent (&event);
	      switch (event.type)
		{
		case SDL_QUIT:
		  cont = 0;
		  break;
		case SDL_KEYDOWN:
		  switch (event.key.keysym.sym)
		    {
		    case SDLK_ESCAPE:
		      cont = 0;
		      break;
		    default:
		      break;
		    }
		}
	    }
	}
      return 0;
      break;

//check if we can mouv the box
    case CELL_BOX:
      switch (player->current_direction)
	{
	case NORTH:
	  if (map_is_inside (map, x, y - 1)
	      && map_get_cell_type (map, x, y - 1) == CELL_EMPTY)
	    {
	      map_set_cell_type (map, x, y - 1,
				 map_get_cell_type2 (map, x, y));
	      map_set_cell_type (map, x, y, CELL_EMPTY);
	      return 1;
	    }
	  break;
	case SOUTH:
	  if (map_is_inside (map, x, y + 1)
	      && map_get_cell_type (map, x, y + 1) == CELL_EMPTY)
	    {
	      map_set_cell_type (map, x, y + 1,
				 map_get_cell_type2 (map, x, y));
	      map_set_cell_type (map, x, y, CELL_EMPTY);
	      return 1;
	    }
	  break;
	case WEST:
	  if (map_is_inside (map, x - 1, y)
	      && map_get_cell_type (map, x - 1, y) == CELL_EMPTY)
	    {
	      map_set_cell_type (map, x - 1, y,
				 map_get_cell_type2 (map, x, y));
	      map_set_cell_type (map, x, y, CELL_EMPTY);
	      return 1;
	    }
	  break;
	case EAST:
	  if (map_is_inside (map, x + 1, y)
	      && map_get_cell_type (map, x + 1, y) == CELL_EMPTY)
	    {
	      map_set_cell_type (map, x + 1, y,
				 map_get_cell_type2 (map, x, y));
	      map_set_cell_type (map, x, y, CELL_EMPTY);
	      return 1;
	    }
	  break;
	}
      return 0;
      break;

    case CELL_BONUS:
      switch (player_bonus (map, x, y))
	{
	case 1:
	  player_inc_r_bomb (player);
	  break;

	case 2:
	  player_dec_r_bomb (player);
	  break;

	case 3:
	  player_dec_nb_bomb (player);
	  break;

	case 4:
	  player_inc_nb_bomb (player);
	  break;

	case 6:
	  player_inc_life (player);
	  break;
	}
      map_set_cell_type (map, x, y, CELL_EMPTY);
      break;

    case CELL_MONSTER:
      if (player->timep == 0)
	{
	  player_dec_life (player);
	  player->timep = SDL_GetTicks ();
	}
      break;

    case CELL_BOMB:
      return 0;
      break;

    case CELL_BOOM:
      if (player->timep == 0)
	{
	  player_dec_life (player);
	  player->timep = SDL_GetTicks ();
	}
      break;

    case CELL_DOOR:
      if ((map_get_cell_type2 (map, x, y) & 00000001) == 0)
	{
	  player->cur_lev++;
	}
      else
	{
	  player->cur_lev--;
	}
      break;

    case CELL_DOOR_CLOSED:
      if (player->key > 0)
	{
	  dec_nb_key (player);
	  map_set_cell_type (map, x, y, map_get_cell_type2 (map, x, y) - 96);
	  if ((map_get_cell_type2 (map, x, y) & 00000001) == 0)
	    {
	      player->cur_lev++;
	    }
	  else
	    {
	      player->cur_lev--;
	    }
	}
      break;
    case CELL_KEY:
      inc_nb_key (player);
      map_set_cell_type (map, x, y, CELL_EMPTY);
      break;

    default:
      break;
    }

  // Player has moved
  return 1;
}

int
player_move (struct player *player, struct map *map)
{
  int x = player->x;
  int y = player->y;
  int move = 0;

  switch (player->current_direction)
    {
    case NORTH:
      if (player_move_aux (player, map, x, y - 1))
	{
	  player->y--;
	  move = 1;
	}
      break;

    case SOUTH:
      if (player_move_aux (player, map, x, y + 1))
	{
	  player->y++;
	  move = 1;
	}
      break;

    case WEST:
      if (player_move_aux (player, map, x - 1, y))
	{
	  player->x--;
	  move = 1;
	}
      break;

    case EAST:
      if (player_move_aux (player, map, x + 1, y))
	{
	  player->x++;
	  move = 1;
	}
      break;
    }

  //if (move) {
  //      map_set_cell_type(map, x, y, CELL_EMPTY);
  //}
  return move;
}

void
player_display (struct player *player)
{
  assert (player);
  if (player->timep == 0)
    {
      window_display_image (sprite_get_player (player->current_direction),
			    player->x * SIZE_BLOC, player->y * SIZE_BLOC);
    }
  else
    {
      int j = SDL_GetTicks ();
      if (j - player->timep < 250
	  || (j - player->timep < 750 && j - player->timep > 500)
	  || (j - player->timep < 1250 && j - player->timep > 1000)
	  || (j - player->timep < 1750 && j - player->timep > 1500))
	{
	  window_display_image (sprite_get_player (player->current_direction),
				player->x * SIZE_BLOC, player->y * SIZE_BLOC);
	}
    }
}

void
player_set_bomb (struct player *player, struct map *map)
{
  assert (player);
  assert (map);
  if (map_get_cell_type (map, player->x, player->y) == CELL_EMPTY
      && player->nb_bombs > 0)
    {
      player->nb_bombs--;
      int i = 0;
      while (i < 20)
	{
	  if (player->tab[i]->timeb == 0)
	    {
	      player->tab[i]->x = player->x;
	      player->tab[i]->y = player->y;
	      player->tab[i]->power = player->power;
	      player->tab[i]->timeb = SDL_GetTicks ();
	      break;
	    }
	  i++;
	}
    }
}

void
chain_explo (int x, int y, int timer, struct player *player)
{
  assert (player);
  int i = 0;
  while (i < 20)
    {
      if ((player->tab[i]->x == x) && (player->tab[i]->y == y))
	{
	  player->tab[i]->timeb = SDL_GetTicks () - timer;
	  break;
	}
      i++;
    }
}

void
bomb_update (struct player *player, struct map *map)
{
  int i = 0;
  int j = SDL_GetTicks ();
  while (i < 20)
    {
      if ((player->tab[i]->timeb) != 0)
	{
	  if (j - player->tab[i]->timeb <= 1000)
	    {
	      map_set_cell_type (map, player->tab[i]->x, player->tab[i]->y,
				 CELL_BOMB_4);
	    }
	  if ((j - player->tab[i]->timeb <= 2000)
	      && (j - player->tab[i]->timeb > 1000))
	    {
	      map_set_cell_type (map, player->tab[i]->x, player->tab[i]->y,
				 CELL_BOMB_3);
	    }
	  if ((j - player->tab[i]->timeb <= 3000)
	      && (j - player->tab[i]->timeb > 2000))
	    {
	      map_set_cell_type (map, player->tab[i]->x, player->tab[i]->y,
				 CELL_BOMB_2);
	    }
	  if ((j - player->tab[i]->timeb <= 4000)
	      && (j - player->tab[i]->timeb > 3000))
	    {
	      map_set_cell_type (map, player->tab[i]->x, player->tab[i]->y,
				 CELL_BOMB_1);
	    }
	  if ((j - player->tab[i]->timeb <= 4500)
	      && (j - player->tab[i]->timeb > 4000))
	    {
	      int power = player->tab[i]->power;
	      int p = 1;
	      map_set_cell_type (map, player->tab[i]->x, player->tab[i]->y,
				 CELL_BOOM);
	      while (p <= power)
		{
		  if (map_is_inside
		      (map, player->tab[i]->x + p, player->tab[i]->y))
		    {
		      if (map_get_cell_type
			  (map, player->tab[i]->x + p,
			   player->tab[i]->y) == CELL_EMPTY
			  || map_get_cell_type (map, player->tab[i]->x + p,
						player->tab[i]->y) ==
			  CELL_BONUS)
			{
			  map_set_cell_type (map, player->tab[i]->x + p,
					     player->tab[i]->y, CELL_BOOM);
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x + p,
			     player->tab[i]->y) == CELL_BOX)
			{
			  if (player_bonus
			      (map, player->tab[i]->x + p, player->tab[i]->y))
			    {	// display the bonus into the box after 500 ms
			      player->tab[i]->box[0]->bonus =
				player_bonus (map, player->tab[i]->x + p,
					      player->tab[i]->y);
			      player->tab[i]->box[0]->x =
				player->tab[i]->x + p;
			      player->tab[i]->box[0]->y = player->tab[i]->y;
			      player->tab[i]->box[0]->timebon = j;
			    }
			  map_set_cell_type (map, player->tab[i]->x + p,
					     player->tab[i]->y, CELL_BOOM);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x + p,
			     player->tab[i]->y) == CELL_SCENERY
			    || map_get_cell_type (map, player->tab[i]->x + p,
						  player->tab[i]->y) ==
			    CELL_KEY)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x + p,
			     player->tab[i]->y) == CELL_BOOM)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x + p,
			     player->tab[i]->y) == CELL_BOMB)
			{
			  chain_explo (player->tab[i]->x + p,
				       player->tab[i]->y, 4001, player);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x + p,
			     player->tab[i]->y) == CELL_MONSTER)
			{
			  map_set_cell_type (map, player->tab[i]->x + p,
					     player->tab[i]->y, CELL_BOOM);
			  monster_reini (map, player->tab[i]->x + p,
					 player->tab[i]->y);
			  break;
			}

		    }
		  p++;
		}
	      p = 1;
	      while (p <= power)
		{
		  if (map_is_inside
		      (map, player->tab[i]->x - p, player->tab[i]->y))
		    {
		      if (map_get_cell_type
			  (map, player->tab[i]->x - p,
			   player->tab[i]->y) == CELL_EMPTY
			  || map_get_cell_type (map, player->tab[i]->x - p,
						player->tab[i]->y) ==
			  CELL_BONUS)
			{
			  map_set_cell_type (map, player->tab[i]->x - p,
					     player->tab[i]->y, CELL_BOOM);
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x - p,
			     player->tab[i]->y) == CELL_BOX)
			{
			  if (player_bonus
			      (map, player->tab[i]->x - p, player->tab[i]->y))
			    {	// display the bonus into the box after 500 ms
			      player->tab[i]->box[1]->bonus =
				player_bonus (map, player->tab[i]->x - p,
					      player->tab[i]->y);
			      player->tab[i]->box[1]->x =
				player->tab[i]->x - p;
			      player->tab[i]->box[1]->y = player->tab[i]->y;
			      player->tab[i]->box[1]->timebon = j;
			    }
			  map_set_cell_type (map, player->tab[i]->x - p,
					     player->tab[i]->y, CELL_BOOM);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x - p,
			     player->tab[i]->y) == CELL_SCENERY
			    || map_get_cell_type (map, player->tab[i]->x - p,
						  player->tab[i]->y) ==
			    CELL_KEY)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x - p,
			     player->tab[i]->y) == CELL_BOOM)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x - p,
			     player->tab[i]->y) == CELL_BOMB)
			{
			  chain_explo (player->tab[i]->x - p,
				       player->tab[i]->y, 4001, player);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x - p,
			     player->tab[i]->y) == CELL_MONSTER)
			{
			  map_set_cell_type (map, player->tab[i]->x - p,
					     player->tab[i]->y, CELL_BOOM);
			  monster_reini (map, player->tab[i]->x - p,
					 player->tab[i]->y);
			  break;
			}
		    }
		  p++;
		}
	      p = 1;
	      while (p <= power)
		{
		  if (map_is_inside
		      (map, player->tab[i]->x, player->tab[i]->y + p))
		    {
		      if (map_get_cell_type
			  (map, player->tab[i]->x,
			   player->tab[i]->y + p) == CELL_EMPTY
			  || map_get_cell_type (map, player->tab[i]->x,
						player->tab[i]->y + p) ==
			  CELL_BONUS)
			{
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y + p,
					     CELL_BOOM);
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y + p) == CELL_BOX)
			{
			  if (player_bonus
			      (map, player->tab[i]->x, player->tab[i]->y + p))
			    {	// display the bonus into the box after 500 ms
			      player->tab[i]->box[2]->bonus =
				player_bonus (map, player->tab[i]->x,
					      player->tab[i]->y + p);
			      player->tab[i]->box[2]->x = player->tab[i]->x;
			      player->tab[i]->box[2]->y =
				player->tab[i]->y + p;
			      player->tab[i]->box[2]->timebon = j;
			    }
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y + p,
					     CELL_BOOM);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y + p) == CELL_SCENERY
			    || map_get_cell_type (map, player->tab[i]->x,
						  player->tab[i]->y + p) ==
			    CELL_KEY)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y + p) == CELL_BOOM)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y + p) == CELL_BOMB)
			{
			  chain_explo (player->tab[i]->x,
				       player->tab[i]->y + p, 4001, player);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y + p) == CELL_MONSTER)
			{
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y + p,
					     CELL_BOOM);
			  monster_reini (map, player->tab[i]->x,
					 player->tab[i]->y + p);
			  break;
			}
		    }
		  p++;
		}
	      p = 1;
	      while (p <= power)
		{
		  if (map_is_inside
		      (map, player->tab[i]->x, player->tab[i]->y - p))
		    {
		      if (map_get_cell_type
			  (map, player->tab[i]->x,
			   player->tab[i]->y - p) == CELL_EMPTY
			  || map_get_cell_type (map, player->tab[i]->x,
						player->tab[i]->y - p) ==
			  CELL_BONUS)
			{
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y - p,
					     CELL_BOOM);
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y - p) == CELL_BOX)
			{
			  if (player_bonus
			      (map, player->tab[i]->x, player->tab[i]->y - p))
			    {	// display the bonus into the box after 500 ms
			      player->tab[i]->box[3]->bonus =
				player_bonus (map, player->tab[i]->x,
					      player->tab[i]->y - p);
			      player->tab[i]->box[3]->x = player->tab[i]->x;
			      player->tab[i]->box[3]->y =
				player->tab[i]->y - p;
			      player->tab[i]->box[3]->timebon = j;
			    }
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y - p,
					     CELL_BOOM);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y - p) == CELL_SCENERY
			    || map_get_cell_type (map, player->tab[i]->x,
						  player->tab[i]->y - p) ==
			    CELL_KEY)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y - p) == CELL_BOOM)
			{
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y - p) == CELL_BOMB)
			{
			  chain_explo (player->tab[i]->x,
				       player->tab[i]->y - p, 4001, player);
			  break;
			}
		      else
			if (map_get_cell_type
			    (map, player->tab[i]->x,
			     player->tab[i]->y - p) == CELL_MONSTER)
			{
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y - p,
					     CELL_BOOM);
			  monster_reini (map, player->tab[i]->x,
					 player->tab[i]->y - p);
			  break;
			}
		    }
		  p++;
		}

	    }
	  if (j - player->tab[i]->timeb > 4500)
	    {
	      int power = player->tab[i]->power;
	      int p = 1;
	      map_set_cell_type (map, player->tab[i]->x, player->tab[i]->y,
				 CELL_EMPTY);
	      while (p <= power)
		{
		  if (map_is_inside
		      (map, player->tab[i]->x + p, player->tab[i]->y))
		    {
		      if ((map_get_cell_type
			   (map, player->tab[i]->x + p,
			    player->tab[i]->y)) == (CELL_BOOM))
			{
			  map_set_cell_type (map, player->tab[i]->x + p,
					     player->tab[i]->y, CELL_EMPTY);
			}
		    }
		  if (map_is_inside
		      (map, player->tab[i]->x - p, player->tab[i]->y))
		    {
		      if ((map_get_cell_type
			   (map, player->tab[i]->x - p,
			    player->tab[i]->y)) == (CELL_BOOM))
			{
			  map_set_cell_type (map, player->tab[i]->x - p,
					     player->tab[i]->y, CELL_EMPTY);
			}
		    }
		  if (map_is_inside
		      (map, player->tab[i]->x, player->tab[i]->y + p))
		    {
		      if ((map_get_cell_type
			   (map, player->tab[i]->x,
			    player->tab[i]->y + p)) == (CELL_BOOM))
			{
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y + p,
					     CELL_EMPTY);
			}
		    }
		  if (map_is_inside
		      (map, player->tab[i]->x, player->tab[i]->y - p))
		    {
		      if ((map_get_cell_type
			   (map, player->tab[i]->x,
			    player->tab[i]->y - p)) == (CELL_BOOM))
			{
			  map_set_cell_type (map, player->tab[i]->x,
					     player->tab[i]->y - p,
					     CELL_EMPTY);
			}
		    }
		  p++;
		}
	      player->nb_bombs++;
	      player->tab[i]->timeb = 0;
	    }
	}
      i++;
    }
}

void
bonus_apparition (struct player *player, struct map *map)
{
  int j = 0;
  int i = 0;
  int k = SDL_GetTicks ();
  while (i < 20)
    {
      while (j < 4)
	{
	  if (k - player->tab[i]->box[j]->timebon > 500)
	    {
	      if (player->tab[i]->box[j]->bonus == 1)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, BONUS_R_INC);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	      else if (player->tab[i]->box[j]->bonus == 2)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, BONUS_R_DEC);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	      else if (player->tab[i]->box[j]->bonus == 3)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, BONUS_N_DEC);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	      else if (player->tab[i]->box[j]->bonus == 4)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, BONUS_N_INC);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	      else if (player->tab[i]->box[j]->bonus == 5)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, CELL_MS);
		  monster_add (map, player->tab[i]->box[j]->x,
			       player->tab[i]->box[j]->y);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	      else if (player->tab[i]->box[j]->bonus == 6)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, BONUS_L);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	      else if (player->tab[i]->box[j]->bonus == 7)
		{
		  map_set_cell_type (map, player->tab[i]->box[j]->x,
				     player->tab[i]->box[j]->y, CELL_KEY);
		  player->tab[i]->box[j]->timebon = 0;
		  player->tab[i]->box[j]->bonus = 0;
		}
	    }
	  j++;
	}
      i++;
    }
}

void
invulnerability (struct player *player, struct map *map)
{
  int i = SDL_GetTicks ();
  if ((map_get_cell_type (map, player->x, player->y) == CELL_BOOM
       || map_get_cell_type (map, player->x, player->y) == CELL_MONSTER)
      && player->timep == 0)
    {
      if (player->life > 1)
	{
	  player_dec_life (player);
	}
      player->timep = i;
    }
  if (i - player->timep > 2000)
    {
      player->timep = 0;
    }
}
