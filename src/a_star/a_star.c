/*

	A* Pathfinding module.

	Based on Patrick Lester's implementation found at www.policyalmanac.org/games/aStarTutorial.htm

	Extended, polished and translated to ansi C by Hans Strömquist, www.frittblas.se

	Source file (a_star.c)

	2014-11-20

*/

#include <stdlib.h>								/* for abs() and malloc()-related functions. */

#include "a_star.h"								/* a_star.c main header */
	

/* arrays */

char walkability[A_STAR_MAP_WIDTH][A_STAR_MAP_HEIGHT] = {0};	/* should reflect the main tilemap or playing-area */
int  open_list[A_STAR_MAP_WIDTH * A_STAR_MAP_HEIGHT + 2];		/* holds IDs of open list items */
int  which_list[A_STAR_MAP_WIDTH + 1][A_STAR_MAP_HEIGHT + 1];	/* used to record whether a cell is in the open list or on the closed list. */
int  open_x[A_STAR_MAP_WIDTH * A_STAR_MAP_HEIGHT + 2];			/* stores the x location of an item in the open list */
int  open_y[A_STAR_MAP_WIDTH * A_STAR_MAP_HEIGHT + 2];			/* stores the y location of an item in the open list */
int  parent_x[A_STAR_MAP_WIDTH + 1][A_STAR_MAP_HEIGHT + 1];		/* stores parent of each cell (x) */
int  parent_y[A_STAR_MAP_WIDTH + 1][A_STAR_MAP_HEIGHT + 1];		/* stores parent of each cell (y) */
int  f_cost[A_STAR_MAP_WIDTH * A_STAR_MAP_HEIGHT + 2];			/* stores F cost of a cell in the open list */
int  g_cost[A_STAR_MAP_WIDTH + 1][A_STAR_MAP_HEIGHT + 1];		/* stores G cost for each cell. */
int  h_cost[A_STAR_MAP_WIDTH * A_STAR_MAP_HEIGHT + 2];			/* stores H cost of a cell in the open list */
int  path_length[A_STAR_NUM_CHARS + 1];							/* stores length of the found path */
int  path_location[A_STAR_NUM_CHARS + 1];						/* stores current position along the chosen path */
int *path_bank[A_STAR_NUM_CHARS + 1];


/* path reading variables */

int path_status[A_STAR_NUM_CHARS + 1];
int x_path[A_STAR_NUM_CHARS + 1];
int y_path[A_STAR_NUM_CHARS + 1];


int on_closed_list = 10;

int a_star_is_init = 0;


/*
 *
 * Allocates memory for path_bank.
 *
 */

int a_star_init()
{

	int i;

	for(i=0; i<A_STAR_NUM_CHARS + 1; i++){
		path_bank[i] = (int*)malloc(4);
		if(path_bank[i] == NULL)
			return A_STAR_OUT_OF_MEM;
	}

	a_star_is_init = 1;

	return 0;

}


/*
 *
 * Deallocates memory of path_bank.
 *
 */

void a_star_close()
{

	int i;

	if(a_star_is_init == 1){
		for(i = 0; i < A_STAR_NUM_CHARS + 1; i++){
			free(path_bank[i]);
		}
		a_star_is_init = 0;
	}

}


/*
 *
 * Sets elements in walkability[][] to A_STAR_WALKABLE or A_STAR_UNWALKABLE.
 *
 */

void a_star_set_walkability(int x, int y, int value)
{

	walkability[x][y] = value;

}


/*
 *
 * Get value at walkability[x][y].
 *
 */

char a_star_get_walkability(int x, int y)
{

	return walkability[x][y];

}


/*
 *
 * Finds a path using A*.
 *
 */

int a_star_find_path(int path_id, int starting_x, int starting_y, int target_x, int target_y, int diagonals)
{

	int on_open_list = 0;
	int parent_x_val = 0;
	int parent_y_val = 0;
	int a = 0;
	int b = 0;
	int m = 0;
	int u = 0;
	int v = 0;
	int temp = 0;
	int corner = 0;
	int num_open_list_items = 0;
	int added_g_cost = 0;
	int temp_g_cost = 0;
	int path = 0;
	int temp_x;
	int path_x;
	int path_y;
	int cell_position;
	int new_open_list_item_id = 0;
	int start_x;
	int start_y;
	int x, y;


	/* 1. Convert location data (in pixels) to coordinates in the walkability array. */

	start_x  = starting_x / A_STAR_TILE_SIZE;
	start_y  = starting_y / A_STAR_TILE_SIZE;
	target_x = target_x   / A_STAR_TILE_SIZE;
	target_y = target_y   / A_STAR_TILE_SIZE;


	/* raise the cost dramatically for diagonals if diagonals == 0, so they don't occur */

	if(diagonals == 0) diagonals = A_STAR_INFINITY;
	else diagonals = 14;


	/* 2. Quick Path Checks: Under the some circumstances no path needs to be generated... */


	/*	If starting location and target are in the same location... */

	if(start_x == target_x && start_y == target_y && path_location[path_id] > 0)
		return A_STAR_FOUND;
	if(start_x == target_x && start_y == target_y && path_location[path_id] == 0)
		return A_STAR_NON_EXISTENT;

	/* If target square is unwalkable, return that it's a nonexistent path. */

	if(walkability[target_x][target_y] == A_STAR_UNWALKABLE)
		goto no_path;

	/* 3. Reset some variables that need to be cleared */

	if(on_closed_list > A_STAR_A_LOT) /* reset which_list occasionally */
	{
		for(x=0; x<A_STAR_MAP_WIDTH; x++){
			for(y=0; y<A_STAR_MAP_HEIGHT; y++)
				which_list[x][y] = 0;
		}
		
		on_closed_list = 10;
		
	}

	on_closed_list = on_closed_list + 2;	/* changing the values of on_open_list and on_closed_list is faster than redimming the which_list[][] array */
	on_open_list = on_closed_list - 1;
	path_length[path_id] = A_STAR_NOT_STARTED;	/* i.e, = 0 */
	path_location[path_id] = A_STAR_NOT_STARTED;
	g_cost[start_x][start_y] = 0;	/* reset starting square's G value to 0 */

	/* 4. Add the starting location to the open list of squares to be checked. */

	num_open_list_items = 1;
	open_list[1] = 1;			/* assign it as the top (and currently only) item in the open list, which is maintained as a binary heap */
	open_x[1] = start_x ;
	open_y[1] = start_y;

	/* 5. Do the following until a path is found or deemed nonexistent. */

	do
	{

	/* 6. If the open list is not empty, take the first cell off of the list. This is the lowest F cost cell on the open list. */

	if (num_open_list_items != 0)
	{

	/* 7. Pop the first item off the open list. */

	parent_x_val = open_x[open_list[1]];
	parent_y_val = open_y[open_list[1]];	/* record cell coordinates of the item */
	which_list[parent_x_val][parent_y_val] = on_closed_list;	/* add the item to the closed list */

/*	Open List = Binary Heap: Delete this item from the open list, which */
/*	is maintained as a binary heap. For more information on binary heaps, see: */
/*	www.policyalmanac.org/games/binaryHeaps.htm */

	num_open_list_items--;	/* reduce number of open list items by 1 */
		
	/* Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top. */

	open_list[1] = open_list[num_open_list_items + 1];	/* move the last item in the heap up to slot #1 */
	v = 1;

	/* Repeat the following until the new item in slot #1 sinks to its proper spot in the heap. */

	do
	{

	u = v;

	if(2 * u + 1 <= num_open_list_items)	/* if both children exist */
	{
	 	/* Check if the F cost of the parent is greater than each child. Select the lowest of the two children. */

		if(f_cost[open_list[u]] >= f_cost[open_list[2 * u]])
			v = 2 * u;
		if(f_cost[open_list[v]] >= f_cost[open_list[2 * u + 1]]) 
			v = 2 * u + 1;
	}
	else
	{
		if(2 * u <= num_open_list_items) /* if only child #1 exists */
		{
	 		/* Check if the F cost of the parent is greater than child #1 */

			if(f_cost[open_list[u]] >= f_cost[open_list[2 * u]]) 
				v = 2 * u;
		}
	}

	if (u != v)	/* if parent's F is > one of its children, swap them */
	{
		temp = open_list[u];
		open_list[u] = open_list[v];
		open_list[v] = temp;			
	}
	else
		break; /* otherwise, exit loop */
		
	}
	while(1); /* reorder the binary heap */


	/* 8. Check the adjacent squares. It's "children" -- these path children are similar, conceptually, to the binary heap children */

	for(b = parent_y_val - 1; b <= parent_y_val + 1; b++){
	for(a = parent_x_val - 1; a <= parent_x_val + 1; a++){

	/* If not off the map (do this first to avoid array out-of-bounds errors) */

	if (a != -1 && b != -1 && a != A_STAR_MAP_WIDTH && b != A_STAR_MAP_HEIGHT){

	/* If not already on the closed list (items on the closed list have already been considered and can now be ignored). */

	if(which_list[a][b] != on_closed_list){
	
	/* If not a wall/obstacle square. */

	if(walkability[a][b] != A_STAR_UNWALKABLE){
		
	/* Don't cut across corners */

	corner = A_STAR_WALKABLE;

	if (a == parent_x_val - 1) 
	{
		if (b == parent_y_val - 1)
		{
			if(walkability[parent_x_val - 1][parent_y_val] == A_STAR_UNWALKABLE || walkability[parent_x_val][parent_y_val - 1] == A_STAR_UNWALKABLE)
				corner = A_STAR_UNWALKABLE;
		}
		else if(b == parent_y_val + 1)
		{
			if(walkability[parent_x_val][parent_y_val + 1] == A_STAR_UNWALKABLE || walkability[parent_x_val - 1][parent_y_val] == A_STAR_UNWALKABLE)
				corner = A_STAR_UNWALKABLE; 
		}
	}
	else if(a == parent_x_val + 1)
	{
		if(b == parent_y_val - 1)
		{
			if(walkability[parent_x_val][parent_y_val - 1] == A_STAR_UNWALKABLE || walkability[parent_x_val + 1][parent_y_val] == A_STAR_UNWALKABLE) 
				corner = A_STAR_UNWALKABLE;
		}
		else if(b == parent_y_val + 1)
		{
			if(walkability[parent_x_val + 1][parent_y_val] == A_STAR_UNWALKABLE || walkability[parent_x_val][parent_y_val + 1] == A_STAR_UNWALKABLE)
				corner = A_STAR_UNWALKABLE; 
		}
	}

	if (corner == A_STAR_WALKABLE){
	
	/* If not already on the open list, add it to the open list. */

	if(which_list[a][b] != on_open_list) 
	{

		/* Create a new open list item in the binary heap. */

		new_open_list_item_id++;			/* each new item has a unique ID# */
		m = num_open_list_items + 1;
		open_list[m] = new_open_list_item_id;	/* place the new open list item (actually, its ID#) at the bottom of the heap */
		open_x[new_open_list_item_id] = a;
		open_y[new_open_list_item_id] = b;		/* record the x and y coordinates of the new item */

		/* Figure out its G cost */

		if (abs(a - parent_x_val) == 1 && abs(b - parent_y_val) == 1)
			added_g_cost = diagonals;	/* cost of going to diagonal squares, "infinite" if diagonals == 0 */
		else	
			added_g_cost = 10;	/* cost of going to non-diagonal squares */

		g_cost[a][b] = g_cost[parent_x_val][parent_y_val] + added_g_cost;

		/* Figure out its H and F costs and parent */

		h_cost[open_list[m]] = 10 * (abs(a - target_x) + abs(b - target_y));
		f_cost[open_list[m]] = g_cost[a][b] + h_cost[open_list[m]];

		parent_x[a][b] = parent_x_val;
		parent_y[a][b] = parent_y_val;

		/* Move the new open list item to the proper place in the binary heap. */
		/* Starting at the bottom, successively compare to parent items, */
		/* swapping as needed until the item finds its place in the heap */
		/* or bubbles all the way to the top (if it has the lowest F cost). */

		while (m != 1)	/* While item hasn't bubbled to the top (m = 1) */
		{

			/* Check if child's F cost is < parent's F cost. If so, swap them. */

			if(f_cost[open_list[m]] <= f_cost[open_list[m / 2]])
			{
				temp = open_list[m / 2];
				open_list[m / 2] = open_list[m];
				open_list[m] = temp;
				m = m / 2;
			}
			else
				break;
		}

		num_open_list_items++;	/* add one to the number of items in the heap */

		/* Change which_list to show that the new item is on the open list. */

		which_list[a][b] = on_open_list;
	}

	/* 9. If adjacent cell is already on the open list, check to see if this */
	/* path to that cell from the starting location is a better one. */
	/* If so, change the parent of the cell and its G and F costs.	*/

	else	/* If which_list[a][b] = on_open_list */
	{
	
		/* Figure out the G cost of this possible new path */

		if (abs(a - parent_x_val) == 1 && abs(b - parent_y_val) == 1)
			added_g_cost = diagonals; /* cost of going to diagonal tiles, "infinity" if diagonals == 0 */
		else	
			added_g_cost = 10; /* cost of going to non-diagonal tiles */

		temp_g_cost = g_cost[parent_x_val][parent_y_val] + added_g_cost;
		
		/* If this path is shorter (G cost is lower) then change the parent cell, G cost and F cost. */

		if (temp_g_cost < g_cost[a][b])	/* if G cost is less */
		{
			parent_x[a][b] = parent_x_val;	/* change the square's parent */
			parent_y[a][b] = parent_y_val;

			g_cost[a][b] = temp_g_cost;		/* change the G cost */

			/* Because changing the G cost also changes the F cost, if */
			/* the item is on the open list we need to change the item's */
			/* recorded F cost and its position on the open list to make */
			/* sure that we maintain a properly ordered open list. */

			for(x = 1; x <= num_open_list_items; x++)	/* look for the item in the heap */
			{
			if (open_x[open_list[x]] == a && open_y[open_list[x]] == b)	/* item found */
			{
				f_cost[open_list[x]] = g_cost[a][b] + h_cost[open_list[x]];	/* change the F cost */
				
				/* See if changing the F score bubbles the item up from it's current location in the heap */

				m = x;

				while (m != 1)	/* While item hasn't bubbled to the top (m = 1) */
				{

					/* Check if child is < parent. If so, swap them. */

					if (f_cost[open_list[m]] < f_cost[open_list[m / 2]])
					{
						temp = open_list[m / 2];
						open_list[m / 2] = open_list[m];
						open_list[m] = temp;
						m = m / 2;
					}
					else
						break;
				} 
				break; /* exit for x = loop */
			}	/* if open_x(open_list[x]) = a */
			}	/* for x = 1 to num_open_list_items */
		}	/* if temp_g_cost < g_cost[a][b] */

	}	/* else if which_list[a][b] = on_open_list */
	}	/* If not cutting a corner */
	}	/* If not a wall/obstacle square. */
	}	/* If not already on the closed list */
	}	/* If not off the map */
	}	/* for (a = parent_x_val - 1; a <= parent_x_val + 1; a++){ */
	}	/* for (b = parent_y_val - 1; b <= parent_y_val + 1; b++){ */

	}	/* if (num_open_list_items != 0) */

	/* 10. If open list is empty then there is no path. */

	else
	{
		path = A_STAR_NON_EXISTENT;
		break;
	}  

	/* If target is added to open list then path has been found. */

	if (which_list[target_x][target_y] == on_closed_list)
	{
		path = A_STAR_FOUND;
		break;
	}

	}
	while(1);	/* Do until path is found or deemed nonexistent */

	/* 11. Save the path if it exists. */

	if (path == A_STAR_FOUND)
	{

	/* A. Working backwards from the target to the starting location by checking */
	/* each cell's parent, figure out the length of the path. */

	path_x = target_x;
	path_y = target_y;

	do
	{

		/* Look up the parent of the current cell. */

		temp_x = parent_x[path_x][path_y];		
		path_y = parent_y[path_x][path_y];

		path_x = temp_x;

		/* Figure out the path length */

		path_length[path_id]++;

	}
	while(path_x != start_x || path_y != start_y);

	/* B. Resize the data bank to the right size in bytes */

	path_bank[path_id] = (int*)realloc(path_bank[path_id], path_length[path_id] * 8);

	if(path_bank[path_id] == NULL) return A_STAR_OUT_OF_MEM;

	/* C. Now copy the path information over to the databank. Since we are */
	/* working backwards from the target to the start location, we copy */
	/* the information to the data bank in reverse order. The result is */
	/* a properly ordered set of path data, from the first step to the last. */

	path_x = target_x;
	path_y = target_y;

	cell_position = path_length[path_id] * 2;	/* start at the end	*/

	do
	{

	cell_position = cell_position - 2;	/* work backwards 2 integers */
	path_bank[path_id] [cell_position] = path_x;
	path_bank[path_id] [cell_position + 1] = path_y;

	/* D. Look up the parent of the current cell. */

	temp_x = parent_x[path_x][path_y];
	path_y = parent_y[path_x][path_y];
	path_x = temp_x;

	/* E. If we have reached the starting square, exit the loop. */

	}
	while (path_x != start_x || path_y != start_y);	

	/* 12. Read the first path step into x_path/y_path arrays */

	a_star_read_path(path_id, starting_x, starting_y, 1);

	}
	return path;


	/* 13. If there is no path to the selected target, set the pathfinder's */
	/* x_path and y_path equal to its current location and return that the path is nonexistent. */

no_path:
	x_path[path_id] = starting_x;
	y_path[path_id] = starting_y;
	return A_STAR_NON_EXISTENT;

}


/*
 *
 * Set path status.
 *
 */

void a_star_set_path_status(int path_id, int value)
{

	path_status[path_id] = value;

}

/*
 *
 * Get path status.
 *
 */

int a_star_get_path_status(int path_id)
{

	return path_status[path_id];

}

/*
 *
 * Get path length.
 *
 */

int a_star_get_path_length(int path_id)
{

	return path_length[path_id];

}

/*
 *
 * Get path location.
 *
 */

int a_star_get_path_location(int path_id)
{

	return path_location[path_id];

}

/*
 *
 * Get x_path.
 *
 */

int a_star_get_x_path(int path_id)
{

	return x_path[path_id];

}

/*
 *
 * Get y_path.
 *
 */

int a_star_get_y_path(int path_id)
{

	return y_path[path_id];

}

/*
 *
 * READ PATH DATA: These functions read the path data and convert
 * it to screen pixel coordinates.
 *
*/

void a_star_read_path(int path_id, int current_x, int current_y, int pixels_per_frame)
{

/*

	Note on pixels_per_frame: 

	This parameter is used to determine if the pathfinder has gotten close
	enough to the center of a given path square to warrant looking up
	the next step on the path.
	 
	This is needed because the speed of certain sprites can
	make reaching the exact center of a path square impossible.
	Imagine an object that has a velocity of 3 pixels per frame. Our
	tile size is 50 pixels, so the center of a tile will be at location
	25, 75, 125, etc. Some of these are not evenly divisible by 3, so
	our pathfinder has to know how close is close enough to the center.
	It calculates this by seeing if the pathfinder is less than 
	pixels_per_frame # of pixels from the center of the square. 

	This could conceivably cause problems if you have a *really* fast
	sprite and/or really small tiles, in which case you may need to
	adjust the formula a bit. But this should almost never be a problem
	for games with standard sized tiles and normal speeds.

*/


	/* If a path has been found for the pathfinder... */

	if (path_status[path_id] == A_STAR_FOUND)
	{

		/* If path finder is just starting a new path or has reached the */
		/* center of the current path square (and the end of the path */
		/* hasn't been reached), look up the next path square. */

		if (path_location[path_id] < path_length[path_id])
		{

			/* if just starting or if close enough to center of square */

			if (path_location[path_id] == 0 || (abs(current_x - x_path[path_id]) < pixels_per_frame && abs(current_y - y_path[path_id]) < pixels_per_frame))
				path_location[path_id]++;

		}

		/* Read the path data */

		x_path[path_id] = a_star_read_path_x(path_id, path_location[path_id]);
		y_path[path_id] = a_star_read_path_y(path_id, path_location[path_id]);

		/* If the center of the last path square on the path has been reached then reset. */

		if (path_location[path_id] == path_length[path_id])
		{

			/* if close enough to center of square */

			if (abs(current_x - x_path[path_id]) < pixels_per_frame && abs(current_y - y_path[path_id]) < pixels_per_frame)
				path_status[path_id] = A_STAR_NOT_STARTED; 

		}


	}

	/* If there is no path for this pathfinder, simply stay in the current location. */

	else
	{	
		x_path[path_id] = current_x;
		y_path[path_id] = current_y;
	}

}


/*
	The following two functions read the raw path data from the path_bank.
	You can call these functions directly and skip the read_path function
	above if you want. Make sure you know what your current path_location is.
*/

/*
 *
 * Reads the x coordinate of the next path step.
 *
 */

int a_star_read_path_x(int path_id, int path_location)
{

	int x;

	if(path_location <= path_length[path_id])
	{

		/* Read coordinate from bank */

		x = path_bank[path_id][path_location * 2 - 2];

		/* Adjust the coordinates so they align with the center of the path square (optional). */

		x = (int)(A_STAR_TILE_SIZE * x);	/* + 0.5 * A_STAR_TILE_SIZE; */
	
	}

	return x;

}	


/*
 *
 * Reads the y coordinate of the next path step.
 *
 */

int a_star_read_path_y(int path_id, int path_location)
{

	int y;

	if (path_location <= path_length[path_id])
	{

		/* Read coordinate from bank */

		y = path_bank[path_id][path_location * 2 - 1];

		y = (int)(A_STAR_TILE_SIZE * y);	/* + 0.5 * A_STAR_TILE_SIZE; */
	
	}
	return y;
}


/* end of a_star.c */
