/*

	A* Pathfinding module.

	Based on Patrick Lester's implementation found at www.policyalmanac.org/games/aStarTutorial.htm

	Extended, polished and translated to ansi C by Hans Strömquist, www.frittblas.se

	Header file (a_star.h)

	2014-11-21

*/

#ifdef __cplusplus
extern "C" {
#endif

/*  change these to fit your project */

#define A_STAR_MAP_WIDTH	60
#define A_STAR_MAP_HEIGHT	34
#define A_STAR_TILE_SIZE	32
#define A_STAR_NUM_CHARS	8

/* return values and stuff */

#define A_STAR_NOT_STARTED	0
#define A_STAR_FOUND		1
#define A_STAR_NON_EXISTENT	2
#define A_STAR_WALKABLE		0
#define A_STAR_UNWALKABLE	1
#define A_STAR_USE_DIAG		1
#define A_STAR_NO_DIAG		0
#define A_STAR_OUT_OF_MEM	3
#define A_STAR_INFINITY		99999
#define A_STAR_A_LOT		1000000


/* function declarations */
	
int  a_star_init(void);
void a_star_close(void);
void a_star_set_walkability(int x, int y, int value);
char a_star_get_walkability(int x, int y);
int  a_star_find_path(int path_id, int starting_x, int starting_y, int target_x, int target_y, int diagonals);
void a_star_set_path_status(int path_id, int value);
int  a_star_get_path_status(int path_id);
int  a_star_get_path_length(int path_id);
int  a_star_get_path_location(int path_id);
int  a_star_get_x_path(int path_id);
int  a_star_get_y_path(int path_id);
void a_star_read_path(int path_id, int current_x, int current_y, int pixels_per_frame);
int  a_star_read_path_x(int path_id, int path_location);
int  a_star_read_path_y(int path_id, int path_location);


#ifdef __cplusplus
}
#endif

/* end of a_star.h */
