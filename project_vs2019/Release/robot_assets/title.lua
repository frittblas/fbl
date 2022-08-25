--
--
--	ROBOT GAME
--
--	title.lua
--
--	Contains title screen stuff
--
--	Hans Strömquist 2021
--
--

-- text
g_title_text = 0
local g_sub_title_text = 0

-- for the circle of robots and circle tween obj
local g_num_spin_robots = 10
local g_robot_angle = 0
local g_circle_radius = 550
local g_circle_obj = {radius = 550} -- goes to 350
local g_circle_tween = 0

-- flux objects
local g_title_obj = {y = -128}
local g_sub_title_obj = {x = -128}
local g_title_tween = 0
local g_sub_title_tween = 0


-- functions

-- void init_title()
-- void exit_title()
-- void tick_title()
-- void present_robots()
-- void spin_robots()


function init_title()

	g_circle_obj.radius = 550
	g_circle_radius = g_circle_obj.radius

	g_title_obj.y = -128
	g_sub_title_obj.x = -128

	-- title
	fbl_set_text_xy(g_title_text, fbl_get_screen_w() / 2, g_title_obj.y)
	fbl_set_text_active(g_title_text, fbl_true)
	g_title_tween = flux.to(g_title_obj, 3, { y = fbl_get_screen_w() / 3 }):ease("elasticout"):delay(1)

	-- sub title
	g_sub_title_text = fbl_create_text(255, 69, 0, 0, "reasonable robots")
	fbl_set_text_align(g_sub_title_text, FBL_ALIGN_CENTER)
	fbl_set_text_xy(g_sub_title_text, g_sub_title_obj.x, fbl_get_screen_h() / 2 + 64)
	g_sub_title_tween = flux.to(g_sub_title_obj, 3, { x = fbl_get_screen_w() / 2 }):ease("elasticout"):delay(2)

	-- elastic circle radius tween
	g_circle_tween = flux.to(g_circle_obj, 3, { radius = 350 }):ease("elasticout"):delay(1)
	
	init_robots()
	
	present_robots()

end

function exit_title()

	-- stop the tweens
	g_title_tween:stop()
	g_sub_title_tween:stop()
	g_circle_tween:stop()

	fbl_set_text_active(g_title_text, fbl_false)
	fbl_delete_text(g_sub_title_text)

	fbl_destroy_all_sprites()

end

function tick_title()

	flux.update(1/60)	-- update tween for 60fps

	-- update position based on tween
	fbl_set_text_xy(g_title_text, fbl_get_screen_w() / 2, g_title_obj.y)
	fbl_set_text_xy(g_sub_title_text, g_sub_title_obj.x, fbl_get_screen_h() / 2 + 64)

	spin_robots()

	if fbl_get_mouse_click(FBLMB_LEFT) > 0 or fbl_get_key_down(4) > 0 then
		--fbl_set_window_mode(1) -- doesn't work in emscripten, or it did but not mobile? yes
		--change_game_state(STATE_RUN_MAZE)
		change_game_state(STATE_EXPLORE)
	end


end

function present_robots()

	local i = 0
	
	-- alarmy
	fbl_create_sprite(32, 0, 32, 32, 0)
	fbl_set_sprite_animation(0, fbl_true, 32, 0, 32, 32, 2, 25, fbl_true)
	-- boingy
	fbl_create_sprite(96, 0, 32, 32, 0)
	fbl_set_sprite_animation(1, fbl_true, 96, 0, 32, 32, 2, 33, fbl_true)
	-- chompy
	fbl_create_sprite(32, 0, 32, 32, 0)
	fbl_set_sprite_animation(2, fbl_true, 160, 0, 32, 32, 3, 25, fbl_true)
	-- dancy
	fbl_create_sprite(256, 0, 32, 32, 0)
	fbl_set_sprite_animation(3, fbl_true, 256, 0, 32, 32, 4, 14, fbl_true)
	-- firebot
	fbl_create_sprite(384, 0, 32, 32, 0)
	fbl_set_sprite_animation(4, fbl_true, 384, 0, 32, 32, 3, 20, fbl_true)
	-- floppy
	fbl_create_sprite(0, 32, 32, 32, 0)
	fbl_set_sprite_animation(5, fbl_true, 0, 32, 32, 32, 2, 100, fbl_true)
	-- levitaty
	fbl_create_sprite(64, 32, 32, 32, 0)
	fbl_set_sprite_animation(6, fbl_true, 64, 32, 32, 32, 7, 33, fbl_true)
	-- necky
	fbl_create_sprite(320, 32, 32, 32, 0)
	fbl_set_sprite_animation(7, fbl_true, 320, 32, 32, 32, 2, 50, fbl_true)
	-- wheely
	fbl_create_sprite(384, 32, 32, 32, 0)
	fbl_set_sprite_animation(8, fbl_true, 384, 32, 32, 32, 2, 14, fbl_true)
	-- partybot
	fbl_create_sprite(0, 64, 32, 32, 0)
	fbl_set_sprite_animation(9, fbl_true, 0, 64, 32, 32, 7, 17, fbl_true)
	
	for i = 0, g_num_spin_robots - 1 do
	
		fbl_set_sprite_scale(i, 1.5)
		
	end

	-- charmy in the middle
	local charmy = fbl_create_sprite(0, 0, 32, 32, 0)
	fbl_set_sprite_scale(charmy, 1.5)
	fbl_set_sprite_xy(charmy, fbl_get_screen_w() / 2 - 24, fbl_get_screen_h() / 3 + 96)
	

end

function spin_robots()

	local x_offset = fbl_get_screen_w() / 2 - 24
	local y_offset = fbl_get_screen_h() / 2 - 24

	g_robot_angle = g_robot_angle + 0.2	-- spin slowly
	
	if g_robot_angle > 360 then g_robot_angle = 0 end

	local tmp = g_robot_angle

	for i = 0, g_num_spin_robots - 1 do
	
		local angle_rad = g_robot_angle * (3.141592654 / 180);
		
		local x = g_circle_radius * math.cos(angle_rad) + x_offset;
		local y = g_circle_radius * math.sin(angle_rad) + y_offset;
		
		fbl_set_sprite_xy(i, x, y)

		g_robot_angle = g_robot_angle + 360 / g_num_spin_robots
		
	end

	g_robot_angle = tmp

	g_circle_radius = g_circle_obj.radius

end
