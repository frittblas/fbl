/*
 *
 *	FBL ENGINE
 *
 *	pathfind.c
 *
 *	Contains pathfinding related functions like adding
 *	paths to sprites and primitives from the a_star module.
 *	I you want to change size of the grid or change number
 *	of characters, you'll change a_star.h and recompile.
 *	Will make this dynamic in the future so you can call
 *	everything from Lua.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"


/* this is just a layer on top of the a_star module. */

int  fbl_pathf_init()
{

	return a_star_init();

}

void fbl_pathf_shutdown()
{

	a_star_close();

}


/*
 * The following four functions return some useful constants from the a_star library.
 */

int fbl_pathf_get_map_w()
{

	return A_STAR_MAP_WIDTH;

}

int fbl_pathf_get_map_h()
{

	return A_STAR_MAP_HEIGHT;

}

int fbl_pathf_get_tile_size()
{

	return A_STAR_TILE_SIZE;

}

int fbl_pathf_get_num_chars()
{

	return A_STAR_NUM_CHARS;

}


/*
 * The following functions just act as an abstratcion layer to the a_star library.
 */


void fbl_pathf_set_walkability(int x, int y, int value)
{

	a_star_set_walkability(x, y, value);

}

char fbl_pathf_get_walkability(int x, int y)
{

	return a_star_get_walkability(x, y);

}

int fbl_pathf_find_path(int path_id, int starting_x, int starting_y, int target_x, int target_y, int diagonals)
{

	return a_star_find_path(path_id, starting_x, starting_y, target_x, target_y, diagonals);

}

void fbl_pathf_set_path_status(int path_id, int value)
{

	a_star_set_path_status(path_id, value);

}

int fbl_pathf_get_path_status(int path_id)
{

	return a_star_get_path_status(path_id);

}

int fbl_pathf_get_path_length(int path_id)
{

	return a_star_get_path_length(path_id);

}

int fbl_pathf_get_path_location(int path_id)
{

	return a_star_get_path_location(path_id);

}

int fbl_pathf_get_x_path(int path_id)
{

	return a_star_get_x_path(path_id);

}

int fbl_pathf_get_y_path(int path_id)
{

	return a_star_get_y_path(path_id);

}

void fbl_pathf_read_path(int path_id, int current_x, int current_y, int pixels_per_frame)
{

	a_star_read_path(path_id, current_x, current_y, pixels_per_frame);

}

int fbl_pathf_read_path_x(int path_id, int path_location)
{

	return a_star_read_path_x(path_id, path_location);

}

int fbl_pathf_read_path_y(int path_id, int path_location)
{

	return a_star_read_path_y(path_id, path_location);

}


/* end of pathfind.c */
