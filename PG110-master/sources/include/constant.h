/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2017-2018"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT	4
#define BANNER_HEIGHT	40

// Time management
#define DEFAULT_GAME_FPS 30

// WINDOW_Constantes
#define WIN_MENU_L 720
#define WIN_MENU_l 576

#define WIN_VIC_L 640
#define WIN_VIC_l 480

#define WIN_GO_L 632
#define WIN_GO_l 576



enum direction {
	NORTH = 0,
	SOUTH=1, // 1
	WEST=2, // 2
	EAST=3, //3
};


#endif /* CONSTANT */
