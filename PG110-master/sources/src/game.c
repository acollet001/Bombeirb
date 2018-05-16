/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
 #include <assert.h>
 #include <time.h>

 #include <game.h>
 #include <misc.h>
 #include <window.h>
 #include <sprite.h>
 #include <player.h>

 struct game {
 	struct map** maps;       // the game's map
 	short max_levels;        // nb maps of the game
 	short current_level;
 	struct player* player;
 	int width;
 	int height;
 };

 struct game*game_load(int lev,int x,int y,int nbmb,int rbmb,int nkey,int life) {
 	sprite_load(); // load sprites into process memory
 	struct game* game = malloc(sizeof(*game));
 	game->maps = malloc(sizeof(struct game));
 	game->max_levels = 8;
 	int j=0;
 	while (j<game->max_levels){
 		game->maps[j] = map_get_static(j,1);
 		j++;
 }
 game->current_level=lev;

 struct map* map = game_get_map(game,lev);
 game->width=map_get_width(map);
 game->height=map_get_width(map);

 game->player = player_init(1);
 player_set_position(game->player, x, y);
 player_set_nb_key(game->player,nkey);
 player_set_nb_bomb(game->player,nbmb);
 player_set_r_bomb(game->player,rbmb);
 player_set_life(game->player,life);
 return game;
 }



 struct game*game_new(void) {
 	sprite_load(); // load sprites into process memory

 	struct game* game = malloc(sizeof(*game));
 	game->maps = malloc(sizeof(struct game));
 	game->max_levels = 8;
 	int i=0;
   while (i<game->max_levels){
 	game->maps[i] = map_get_static(i,0);
 	i++;
 }
 game->current_level = 0;
 struct map* map = game_get_current_map(game);
 game->width=map_get_width(map);
 game->height=map_get_width(map);



 	game->player = player_init(1);
 	// Set default location of the player
 	player_set_position(game->player, 1, 0);

 	return game;
 }

 int game_get_current_level(struct game* game){
 	assert(game);
 	return game->current_level;
 }

 void set_game_level(struct game*game,int i){
 	assert(game);
 	game->current_level=i;
 }

 void game_free(struct game* game) {
 	assert(game);

 	player_free(game->player);
 	for (int i = 0; i < game->max_levels; i++)
 		map_free(game->maps[i]);
 }

 struct map* game_get_current_map(struct game* game) {
 	assert(game);
 	return game->maps[game->current_level];
 }

 struct map* game_get_map(struct game* game,int i){
 	assert(game);
 	return game->maps[i];
 }


 struct player* game_get_player(struct game* game) {
 	assert(game);
 	return game->player;
 }

 void game_banner_display(struct game* game) {
 	assert(game);

 	struct map* map = game_get_current_map(game);

 	int y = (map_get_height(map)) * SIZE_BLOC;
 	for (int i = 0; i < map_get_width(map); i++)
 		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

 	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 8 * SIZE_BLOC) / 5;
 	int x = white_bloc;
 	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
 	window_display_image(sprite_get_banner_life(), x, y);

 	x = white_bloc + SIZE_BLOC;
 	window_display_image(sprite_get_number(player_get_life(game_get_player(game))), x, y);

 	x = 2 * white_bloc + 2 * SIZE_BLOC;
 	window_display_image(sprite_get_banner_bomb(), x, y);

 	x = 2 * white_bloc + 3 * SIZE_BLOC;
 	window_display_image(
 			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

 	x = 3 * white_bloc + 4 * SIZE_BLOC;
 	window_display_image(sprite_get_banner_range(), x, y);

 	x = 3 * white_bloc + 5 * SIZE_BLOC;
 	window_display_image(sprite_get_number(game_get_current_level(game)), x, y);

 	x=4* white_bloc + 6*SIZE_BLOC;
 	window_display_image(sprite_get_key(), x, y);
 	x=4* white_bloc + 7*SIZE_BLOC;
 	window_display_image(sprite_get_number(player_get_key(game_get_player(game))), x, y);

 }

 void game_display(struct game* game) {
 	assert(game);

 	window_clear();

 	game_banner_display(game);
 	map_display(game_get_current_map(game));
 	player_display(game->player);

 	window_refresh();
 }

 static short input_keyboard(struct game* game) {
 	SDL_Event event;
 	struct player* player = game_get_player(game);
 	struct map* map = game_get_current_map(game);

 	while (SDL_PollEvent(&event)) {
 		switch (event.type) {
 		case SDL_QUIT:
 			return 1;
 		case SDL_KEYDOWN:
 			switch (event.key.keysym.sym) {
 			case SDLK_ESCAPE:
 				return 1;
 			case SDLK_UP:
 				player_set_current_way(player, NORTH);
 				player_move(player, map);
 				break;
 			case SDLK_DOWN:
 				player_set_current_way(player, SOUTH);
 				player_move(player, map);
 				break;
 			case SDLK_RIGHT:
 				player_set_current_way(player, EAST);
 				player_move(player, map);
 				break;
 			case SDLK_LEFT:
 				player_set_current_way(player, WEST);
 				player_move(player, map);
 				break;
 			case SDLK_SPACE:
 				player_set_bomb(player,map);
 				break;
 			case SDLK_s:
 				player_save_game(game);
 				break;
 			default:
 				break;
 			}

 			break;
 		}
 	}
 	return 0;
 }

 void update_map(struct game*game){
 	struct player* player = game_get_player(game);
 	int p=player_cur_level(player);
 	game->current_level=p;
 }

 void update_window(struct game*game){
 	struct map* map = game_get_current_map(game);
 	int WIDTH=map_get_width(map);
 	int HEIGHT=map_get_height(map);
 	if ((WIDTH!=game->width) || (HEIGHT!=game->height)){
 		window_create(SIZE_BLOC * WIDTH,
 		SIZE_BLOC * HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);
 		game->width=WIDTH;
 		game->height=HEIGHT;
 	}
 }


 int game_update(struct game* game) {
 	struct player* player = game_get_player(game);
 	struct map* map = game_get_current_map(game);
 	bomb_update(player,map);
 	monster_update(map);
   bonus_apparition(player,map);
 	invulnerability(player,map);
 	update_map(game);
   update_window(game);
 	if (input_keyboard(game))
 		return 1; // exit game
 	return 0;
 }


 void player_save_game(struct game * game){
 	FILE* save= NULL;
 	struct player* player = game_get_player(game);
 	int worlds = game->max_levels;
 	int cur_lev =game->current_level;
 	int x=player_get_x(player);
 	int y=player_get_y(player);
 	int nbmb=player_get_nb_bomb(player);
 	int rbmb=player_get_r_bomb(player);
 	int nkey=player_get_key(player);
 	int life=player_get_life(player);
 	save=fopen("data/save.txt","w+");
 	fprintf(save,"%d \n",worlds);
 	fprintf(save,"%d,%d,%d \n",cur_lev,x,y);
 	fprintf(save,"%d ,%d ,%d ,%d \n",nbmb,rbmb,nkey,life);
 	fclose(save);
 	int i=0;
 	while (i<worlds){
 		struct map* map = game_get_map(game,i);
 		char chaine[21];
 		FILE* level= NULL;
 		sprintf(chaine,"data/map_saved_%d.txt",i);
 		level=fopen(chaine,"w+");
 		int width=map_get_width(map);
 		int height=map_get_height(map);
 		int j=0;
 		int k=0;
 		fprintf(level,"%d x %d\n",width,height);
 		while (j<height){
 			while(k<width){
 				int cell=map_get_cell_type3(map,k+j*width);
 				fprintf(level,"%d ",cell);
 				k++;
 						}
 						j++;
 						k=0;
 			fprintf(level,"\n");
 		}
 		fclose(level);
 		i++;
 	}
 }
