--
--
--	ROBOT GAME
--
--	maze.lua
--
--	Contains maze related functions
--
--	Hans Strömquist 2021
--
--


TILE_SIZE = 32

MAP_SIZE_X = 25
MAP_SIZE_Y = 25

TARGET_X = 12 * TILE_SIZE
TARGET_Y = 12 * TILE_SIZE


local g_game_map = {}	-- create the 2d playing field matrix


-- for the pre maze pick

g_get_ready_text = 0 -- get ready text id

local GET_READY = 1	-- seconds of GET READY text with black bg
local g_time_to_pick = 5	-- seconds of actual time player has to pick corner

local g_pick_timer = 0	-- timer for picking state (in seconds * 60)
local g_picked_position = 0	-- stores the picked corner (1 = up left, 2 = up right, 3 down left, 4 down right)

local g_black_bg = 0	-- id for black background id
local g_black_bg_fade = 255 -- bg fadout

local g_time_bar = 0	-- green time bar id
local g_time_bar_red = 0 -- how much red in bar

local g_circle_size = {} -- for the circle effect


-- functions

-- void init_maze()
-- void exit_maze()
-- void tick_maze()
-- void setup_pick_start()
-- void pick_start_position()
-- void find_paths()
-- void reset_maze()
-- void randomize_maze()
-- void populate_maze()
-- void bool maze_has_all_paths()
-- assign_paths()


function init_maze()

	setup_pick_start()
	find_paths()

end

function exit_maze()

	fbl_destroy_all_sprites()

end

function tick_maze()

	if g_pick_timer > 0 then pick_start_position()
	else
		
		if g_pick_timer < -150 then	-- wait a little before starting
			tick_robots()
		else
			g_pick_timer = g_pick_timer - 1
		end
		
		--print(g_picked_position)
		
	end

	-- Q(uit)
	if fbl_get_key_down(FBLK_Q) > 0 then
		if g_pick_timer < 1 then -- can only quit after corner pick
			change_game_state(STATE_TITLE)
		end
	end

end

function setup_pick_start()

	-- black bg, rect = 3

	g_black_bg = fbl_create_prim(3, 0, 0, fbl_get_screen_w(), MAP_SIZE_Y * TILE_SIZE, 0, 0, 1)
	fbl_set_prim_color(g_black_bg, 0, 0, 0, 255)
	g_black_bg_fade = 255

	fbl_set_text_active(g_get_ready_text, fbl_true) -- show text

	-- circle = type 6 (starts at id's 1-11)

	g_circle_size[0] = 5
	g_circle_size[1] = 30
	g_circle_size[2] = 55

	fbl_create_prim(FBL_CIRCLE, TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, g_circle_size[0], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, g_circle_size[1], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, g_circle_size[2], fbl_true, fbl_false)

	fbl_create_prim(FBL_CIRCLE, fbl_get_screen_w() - TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, g_circle_size[0], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, fbl_get_screen_w() - TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, g_circle_size[1], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, fbl_get_screen_w() - TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, g_circle_size[2], fbl_true, fbl_false)

	fbl_create_prim(FBL_CIRCLE, TILE_SIZE / 2, MAP_SIZE_Y * TILE_SIZE - TILE_SIZE / 2, 0, 0, g_circle_size[0], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, TILE_SIZE / 2, MAP_SIZE_Y * TILE_SIZE - TILE_SIZE / 2, 0, 0, g_circle_size[1], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, TILE_SIZE / 2, MAP_SIZE_Y * TILE_SIZE - TILE_SIZE / 2, 0, 0, g_circle_size[2], fbl_true, fbl_false)

	fbl_create_prim(FBL_CIRCLE, fbl_get_screen_w() - TILE_SIZE / 2, MAP_SIZE_Y * TILE_SIZE - TILE_SIZE / 2, 0, 0, g_circle_size[0], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, fbl_get_screen_w() - TILE_SIZE / 2, MAP_SIZE_Y * TILE_SIZE - TILE_SIZE / 2, 0, 0, g_circle_size[1], fbl_true, fbl_false)
	fbl_create_prim(FBL_CIRCLE, fbl_get_screen_w() - TILE_SIZE / 2, MAP_SIZE_Y * TILE_SIZE - TILE_SIZE / 2, 0, 0, g_circle_size[2], fbl_true, fbl_false)

	g_time_bar = fbl_create_prim(FBL_RECT, fbl_get_screen_w() / 2, (MAP_SIZE_Y + 1) * TILE_SIZE, g_time_to_pick * 60, TILE_SIZE / 2, 0, fbl_false, fbl_true)
	fbl_set_prim_color(g_time_bar, 0, 255, 0, 255)

	g_time_bar_red = 0

	g_pick_timer = (GET_READY + g_time_to_pick) * 60 -- 8 seconds (i.e 3 + 5)


end

function pick_start_position()

	local i

	-- do some nice circle effects

	for i = 0, 2 do
	
		g_circle_size[i] = g_circle_size[i] + 1
		if g_circle_size[i] > 55 then g_circle_size[i] = 5 end
		
	end

	for i = 1, 12 do	-- this demands that the circle prims are created starting at id 1
	
		if i == 1 or i == 4 or i == 7 or i == 10 then fbl_set_prim_size(i, 0, 0, g_circle_size[0])
		elseif i == 2 or i == 5 or i == 8 or i == 11 then fbl_set_prim_size(i, 0, 0, g_circle_size[1])
		elseif i == 3 or i == 6 or i == 9 or i == 12 then fbl_set_prim_size(i, 0, 0, g_circle_size[2]) end
		
	end


	-- handle pick selection

	if fbl_get_mouse_click(FBLMB_LEFT) > 0 and g_pick_timer < g_time_to_pick * 60 then	-- if clicked and after GET READY phase 
	
		if fbl_get_mouse_x() < 96 and fbl_get_mouse_y() < 96 then	-- up left
			g_picked_position = 0	-- this is the picked corner
			g_pick_timer = -1 -- stop the pick corner state
			assign_paths()	-- assign new paths to the robots based on the selection
		elseif fbl_get_mouse_x() > fbl_get_screen_w() - 96 and fbl_get_mouse_y() < 96 then
			g_picked_position = 1
			g_pick_timer = -1
			assign_paths()
		elseif fbl_get_mouse_x() < 96 and fbl_get_mouse_y() > MAP_SIZE_Y * TILE_SIZE - 96 then
			g_picked_position = 2
			g_pick_timer = -1
			assign_paths()
		elseif fbl_get_mouse_x() > fbl_get_screen_w() - 96 and fbl_get_mouse_y() > MAP_SIZE_Y * TILE_SIZE - 96 then
			g_picked_position = 3
			g_pick_timer = -1
			assign_paths()
		end
	
	end

	g_pick_timer = g_pick_timer - 1	-- count down
	
	if g_pick_timer < (g_time_to_pick + 1) * 60 then
		g_black_bg_fade = g_black_bg_fade - 4
		fbl_set_prim_color(g_black_bg, 0, 0, 0, g_black_bg_fade)
	end
	
	if g_pick_timer == g_time_to_pick * 60 then
		fbl_set_prim_active(g_black_bg, fbl_false)	-- deactivate bg, all prims get destroyed after countdown
		fbl_set_text_active(g_get_ready_text, fbl_false)
	end
	
	-- time bar
	if g_pick_timer < g_time_to_pick * 60 then
		fbl_set_prim_size(g_time_bar, g_pick_timer, TILE_SIZE / 2)
		local green_fade = g_pick_timer -- 45
		if green_fade > 255 then green_fade = 255 end
		if green_fade < 1 then green_fade = 0 end
		fbl_set_prim_color(g_time_bar, g_time_bar_red, green_fade, 0, 255)
		g_time_bar_red = g_time_bar_red + 1
		if g_time_bar_red > 255 then g_time_bar_red = 255 end
		--print(g_pick_timer - 45)
	end
	


	if g_pick_timer < 1 then
		if g_pick_timer == 0 then
			g_picked_position = math.random(4) - 1 -- if time runs out, assign a random position
			assign_paths()
		end
		fbl_destroy_all_prims()
		fbl_set_text_active(g_get_ready_text, fbl_false)
	end


end


function find_paths()

	local tries = 0 -- number of brute force tries
	local i
	
	for i = 0, NUM_ROBOTS do
		fbl_pathf_set_path_status(i, 0)
	end
	
	-- the robots starting coordinates
	g_robot[PLAYER][R_X] = 0
	g_robot[PLAYER][R_Y] = 0
	g_robot[SPINKY][R_X] = (MAP_SIZE_X - 1) * TILE_SIZE
	g_robot[SPINKY][R_Y] = 0
	g_robot[DIETER][R_X] = 0
	g_robot[DIETER][R_Y] = (MAP_SIZE_Y - 1) * TILE_SIZE
	g_robot[KARIN][R_X]  = (MAP_SIZE_X - 1) * TILE_SIZE
	g_robot[KARIN][R_Y]  = (MAP_SIZE_Y - 1) * TILE_SIZE

	local USE_DIAG = FBL_PATHF_NO_DIAG -- 0

	repeat
		
		reset_maze()
		randomize_maze(40)

		fbl_pathf_set_path_status(0, fbl_pathf_find_path(0, g_robot[PLAYER][R_X], g_robot[PLAYER][R_Y], TARGET_X, TARGET_Y, USE_DIAG))
		fbl_pathf_set_path_status(1, fbl_pathf_find_path(1, g_robot[SPINKY][R_X], g_robot[SPINKY][R_Y], TARGET_X, TARGET_Y, USE_DIAG))
		fbl_pathf_set_path_status(2, fbl_pathf_find_path(2, g_robot[DIETER][R_X], g_robot[DIETER][R_Y], TARGET_X, TARGET_Y, USE_DIAG))
		fbl_pathf_set_path_status(3, fbl_pathf_find_path(3, g_robot[KARIN][R_X], g_robot[KARIN][R_Y], TARGET_X, TARGET_Y, USE_DIAG))
		
		tries = tries + 1
		
	until maze_has_all_paths()
	
	-- set relevant tiles to walkable
	
	fbl_pathf_set_walkability(g_robot[PLAYER][R_X] / TILE_SIZE, g_robot[PLAYER][R_Y] / TILE_SIZE, FBL_PATHF_WALKABLE)
	fbl_pathf_set_walkability(g_robot[SPINKY][R_X] / TILE_SIZE, g_robot[SPINKY][R_Y] / TILE_SIZE, FBL_PATHF_WALKABLE)
	fbl_pathf_set_walkability(g_robot[DIETER][R_X] / TILE_SIZE, g_robot[DIETER][R_Y] / TILE_SIZE, FBL_PATHF_WALKABLE)
	fbl_pathf_set_walkability(g_robot[KARIN][R_X] / TILE_SIZE, g_robot[KARIN][R_Y] / TILE_SIZE, FBL_PATHF_WALKABLE)
	
	fbl_pathf_set_walkability(TARGET_X / TILE_SIZE, TARGET_Y / TILE_SIZE, FBL_PATHF_WALKABLE)
	
	
	-- create sprites
	fbl_create_sprite(0, 0, 32, 32, 0)	-- player (ID 0)
	--fbl_set_sprite_xy(PLAYER, -24, -24)
	fbl_set_sprite_xy(PLAYER, -64, -64)
	fbl_set_sprite_scale(PLAYER, 1.5)
	fbl_set_sprite_layer(PLAYER, 1)
	
	fbl_create_sprite(0, 0, 32, 32, 0)	-- spinky (ID 1)
	--fbl_set_sprite_xy(SPINKY, g_robot[SPINKY][R_X], 0)
	fbl_set_sprite_xy(SPINKY, -64, -64)
	fbl_set_sprite_animation(SPINKY, 1, 256, 0, 32, 32, 4, 14, 1)
	fbl_set_sprite_scale(SPINKY, 1.5)
	fbl_set_sprite_layer(SPINKY, 1)
	--fbl_set_sprite_color(SPINKY, 200, 0, 200)
	
	fbl_create_sprite(0, 0, 32, 32, 0)	-- dieter (ID 2)
	--fbl_set_sprite_xy(DIETER, 0, g_robot[DIETER][R_Y])
	fbl_set_sprite_xy(DIETER, -64, -64)
	fbl_set_sprite_animation(DIETER, 1, 160, 0, 32, 32, 3, 25, 1)
	fbl_set_sprite_scale(DIETER, 1.5)
	fbl_set_sprite_layer(DIETER, 1)
	--fbl_set_sprite_color(DIETER, 200, 200, 0)
	
	fbl_create_sprite(0, 0, 32, 32, 0)	-- karin (ID 3)
	--fbl_set_sprite_xy(KARIN, g_robot[KARIN][R_X], g_robot[KARIN][R_Y])
	fbl_set_sprite_xy(KARIN, -64, -64)
	fbl_set_sprite_animation(KARIN, 1, 64, 32, 32, 32, 7, 33, 1)
	fbl_set_sprite_scale(KARIN, 1.5)
	fbl_set_sprite_layer(KARIN, 1)
	--fbl_set_sprite_color(KARIN, 0, 200, 200)
	
	
	-- target tile
	fbl_create_sprite(0, 2 * TILE_SIZE, 32, 32, 0)	-- ID 4
	fbl_set_sprite_xy(4, TARGET_X, TARGET_Y)
	
	populate_maze()
	
	fbl_sort_sprites(FBL_SORT_BY_LAYER)	-- NOTE: remove the print statement in fbl update sprite refs (sprite.c)

--[[
	print(fbl_get_num_sprites())
	
	
	print(fbl_pathf_get_path_status(0))
	print(fbl_pathf_get_path_status(1))
	print(fbl_pathf_get_path_status(2))
	print(fbl_pathf_get_path_status(3))

	print(maze_has_all_paths())
--]]

	print("tries: ", tries)

end


function reset_maze()

	local i, j

	for i = 0, MAP_SIZE_X - 1 do
		g_game_map[i] = {}	-- create a new row
		for j = 0, MAP_SIZE_Y - 1 do
			g_game_map[i][j] = FBL_PATHF_WALKABLE
			fbl_pathf_set_walkability(i, j, FBL_PATHF_WALKABLE) -- set all fields to walkable
		end
	end

end


function randomize_maze(density)

	local i, j

	for i = 0, MAP_SIZE_X - 1 do
		for j = 0, MAP_SIZE_Y - 1 do
			if math.random(100) < density then
				g_game_map[i][j] = FBL_PATHF_UNWALKABLE
				fbl_pathf_set_walkability(i, j, FBL_PATHF_UNWALKABLE) -- set these to unwalkable
			end
			--print(fbl_pathf_get_walkability(i, j))
		end

	end

end


function populate_maze()

	local i, j

	-- creates sprites with correct coordinates as obstacles

	for i = 0, MAP_SIZE_X - 1 do
		for j = 0, MAP_SIZE_Y - 1 do

			if fbl_pathf_get_walkability(i, j) == FBL_PATHF_UNWALKABLE then

				local id = fbl_create_sprite(0, 96, 32, 32, 0)
				fbl_set_sprite_xy(id, i * TILE_SIZE, j * TILE_SIZE)
				--fbl_set_sprite_layer(i, -10)

			end
		
		end

	end

end

function maze_has_all_paths()

	if  fbl_pathf_get_path_status(0) == FBL_PATHF_FOUND and
		fbl_pathf_get_path_status(1) == FBL_PATHF_FOUND and
		fbl_pathf_get_path_status(2) == FBL_PATHF_FOUND and
		fbl_pathf_get_path_status(3)  == FBL_PATHF_FOUND then
		return true
	else
		return false
	end

end

function assign_paths()

	-- randomize paths, then assign g_picked_position to the player and position everything
	
	local list = {0, 1, 2, 3}	-- create ordered array with path id's as values
	local i = 0
	
--[[
	for i = 1, #list do
		print(list[i])
	end
--]]
	
	-- shuffle the array using the Fisher-Yates algorithm
	for i = #list, 2, -1 do
		local j = math.random(i)
		list[i], list[j] = list[j], list[i]
	end
	
--[[
	print("efter")
	for i = 1, #list do
		print(list[i])
	end
--]]

	-- assign unique random path id's to the robots
	g_robot[PLAYER][R_PATH] = list[1]
	g_robot[SPINKY][R_PATH] = list[2]
	g_robot[DIETER][R_PATH] = list[3]
	g_robot[KARIN][R_PATH] = list[4]


	-- find the robot who has the picked path, and swap it with the players path (even if it's the player)
	for i = 0, NUM_ROBOTS - 1 do
	
		if g_robot[i][R_PATH] == g_picked_position then	-- found it
		
			-- swap
			g_robot[i][R_PATH] = g_robot[PLAYER][R_PATH]
			g_robot[PLAYER][R_PATH] = g_picked_position
			
			break	-- break loop
		
		end
	
	end


	-- set new coordinates for the robots
	for i = 0, NUM_ROBOTS - 1 do
	
		if g_robot[i][R_PATH] == 0 then
			g_robot[i][R_X] = 0
			g_robot[i][R_Y] = 0
		elseif g_robot[i][R_PATH] == 1 then
			g_robot[i][R_X] = (MAP_SIZE_X - 1) * TILE_SIZE
			g_robot[i][R_Y] = 0
		elseif g_robot[i][R_PATH] == 2 then
			g_robot[i][R_X] = 0
			g_robot[i][R_Y] = (MAP_SIZE_Y - 1) * TILE_SIZE
		elseif g_robot[i][R_PATH] == 3 then
			g_robot[i][R_X]  = (MAP_SIZE_X - 1) * TILE_SIZE
			g_robot[i][R_Y]  = (MAP_SIZE_Y - 1) * TILE_SIZE
		end
	
	end

	-- place_robots (including 8 px offset)

	fbl_set_sprite_xy(PLAYER, g_robot[PLAYER][R_X] - 8, g_robot[PLAYER][R_Y] - 8)
	fbl_set_sprite_xy(SPINKY, g_robot[SPINKY][R_X] - 8, g_robot[SPINKY][R_Y] - 8)
	fbl_set_sprite_xy(DIETER, g_robot[DIETER][R_X] - 8, g_robot[DIETER][R_Y] - 8)
	fbl_set_sprite_xy(KARIN, g_robot[KARIN][R_X] - 8, g_robot[KARIN][R_Y] - 8)


end
