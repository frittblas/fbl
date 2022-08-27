--
--
--	ROBOT GAME
--
--	robot.lua
--
--	Contains robot stuff
--
--	Hans Strömquist 2021
--
--

NUM_ROBOTS = 4

-- robot names
PLAYER = 0
SPINKY = 1
DIETER = 2
KARIN =  3

NUM_ROBOT_DATA = 4

R_X = 0
R_Y = 1
R_SPEED = 2
R_PATH =  3	-- path id


g_robot = {}	-- global robot table containing all robot data


-- functions

-- void init_robots()
-- void tick_robots()
-- void move_robots()
-- void control_bot()


function init_robots()

	local i, j

	-- init robot table to zero
	for i = 0, NUM_ROBOTS - 1 do

		g_robot[i] = {}	-- create a new row

		for j = 0, NUM_ROBOT_DATA - 1 do
			g_robot[i][j] = 0
		end

	end
	
	-- init with reasonable values
	
	for i = 0, NUM_ROBOTS - 1 do
		g_robot[i][R_SPEED] = math.random(1.0)
	end
	
	g_robot[0][R_PATH] = 0
	g_robot[1][R_PATH] = 1
	g_robot[2][R_PATH] = 2
	g_robot[3][R_PATH] = 3

	
end


function tick_robots()

	move_robots()

end


-- move the robots using pathfinding

function move_robots()

	local i

	for i = 0, NUM_ROBOTS - 1 do

		fbl_pathf_read_path(g_robot[i][R_PATH], g_robot[i][R_X], g_robot[i][R_Y], 1)

		if g_robot[i][R_X] > fbl_pathf_get_x_path(g_robot[i][R_PATH]) then g_robot[i][R_X] = g_robot[i][R_X] - g_robot[i][R_SPEED] end
		if g_robot[i][R_X] < fbl_pathf_get_x_path(g_robot[i][R_PATH]) then g_robot[i][R_X] = g_robot[i][R_X] + g_robot[i][R_SPEED] end
		if g_robot[i][R_Y] > fbl_pathf_get_y_path(g_robot[i][R_PATH]) then g_robot[i][R_Y] = g_robot[i][R_Y] - g_robot[i][R_SPEED] end
		if g_robot[i][R_Y] < fbl_pathf_get_y_path(g_robot[i][R_PATH]) then g_robot[i][R_Y] = g_robot[i][R_Y] + g_robot[i][R_SPEED] end

		fbl_set_sprite_xy(i, g_robot[i][R_X] - 8, g_robot[i][R_Y] - 8)	-- offset the robots coordinates slightly to fit the scaled sprites
		
	end
	
	control_bot()

end


-- control robot with WASD

function control_bot()


	if fbl_get_key_down(FBLK_W) > 0 then fbl_set_sprite_xy(0, fbl_get_sprite_x(0), fbl_get_sprite_y(0) - 2) end
	if fbl_get_key_down(FBLK_S) > 0 then fbl_set_sprite_xy(0, fbl_get_sprite_x(0), fbl_get_sprite_y(0) + 2) end
	if fbl_get_key_down(FBLK_A) > 0 then fbl_set_sprite_xy(0, fbl_get_sprite_x(0) - 2, fbl_get_sprite_y(0)) end
	if fbl_get_key_down(FBLK_D) > 0 then fbl_set_sprite_xy(0, fbl_get_sprite_x(0) + 2, fbl_get_sprite_y(0)) end
	

end
