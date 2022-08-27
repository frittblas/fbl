--
--
--	ROBOT GAME
--
--	main.lua
--
--	This will be called from your C-program if you call fbl_lua_init()
--	in the beginning of your program. First, the fbl_lua_start(),
--	then, fbl_lua_loop() will be called every frame. Last, fbl_lua_end()
--	will be called when you end the program from C to clean stuff up.
--
--	You have full access to the fbl api from here!
--
--	For reference, fbl.h is handy for function definitions and constants
--	and stuff. In functions where fbl wants a bool, just pass 1 for true,
--	and 0 for false.
--
--	Hans Strömquist 2021
--
--



-- these need the path to emscripten to work when built with emscripten (E.g "fbl/project_emscripten/output/") to work
-- prefix with PATH_TO_EMSCRIPTEN_ASSETS in fbl.lua
-- Or just uncomment the next block and comment out the following


--[[
require "fbl/project_emscripten/output/robot_assets/strictvar"
require "fbl/project_emscripten/output/robot_assets/fbl"
require "fbl/project_emscripten/output/robot_assets/maze"
require "fbl/project_emscripten/output/robot_assets/game_state"
require "fbl/project_emscripten/output/robot_assets/title"
require "fbl/project_emscripten/output/robot_assets/robot"
require "fbl/project_emscripten/output/robot_assets/skill"
require "fbl/project_emscripten/output/robot_assets/explore"
require "fbl/project_emscripten/output/robot_assets/dialogue"
flux = require "fbl/project_emscripten/output/robot_assets/addons/flux/flux"
--]]

require "robot_assets/strictvar"
require "robot_assets/fbl"
require "robot_assets/maze"
require "robot_assets/game_state"
require "robot_assets/title"
require "robot_assets/robot"
require "robot_assets/skill"
require "robot_assets/explore"
require "robot_assets/dialogue"
flux = require "robot_assets/addons/flux/flux"	-- this could go first to avoid strict! :) test it

-- functions

-- fbl_lua_start()
-- fbl_lua_loop()
-- fbl_lua_end()


function fbl_lua_start()

	print("Lua start!\n")
	
	make_strict()
	
	fbl_set_clear_color(1, 68, 33, 255)
	--fbl_set_screen_wh(800, 864)	-- 25 x 24 (32 pixel tiles, with 96 pixels free at the bottom)
	
	fbl_set_system_delay_ms(10)
	
	math.randomseed(os.time())
	
	-- init the a_star library
	if fbl_pathf_init() == FBL_PATHF_OUT_OF_MEM then
		-- do stuff
	end
	
	fbl_load_texture("gfx/sheet.png")

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER)	-- sprites are drawn from top left corner???????????


	fbl_load_ui_texture("gfx/ui_1.png")	-- load ui texture
	
	-- big font normal
	fbl_load_ttf_font("font/roboto.ttf", 50)
	
	-- title text
	g_title_text = fbl_create_text(0, 200, 0, 0, "Charming Alarming 2")
	fbl_set_text_align(g_title_text, FBL_ALIGN_CENTER)
	fbl_set_text_xy(g_title_text, fbl_get_screen_w() / 2, fbl_get_screen_h() / 3)
	
	-- get ready text
	g_get_ready_text = fbl_create_text(255, 69, 0, 255, "GET READY!")
	fbl_set_text_align(g_get_ready_text, FBL_ALIGN_CENTER)
	fbl_set_text_xy(g_get_ready_text, fbl_get_screen_w() / 2, fbl_get_screen_h() / 3)
	fbl_set_text_active(g_get_ready_text, fbl_false)


	-- small font italic
	fbl_load_ttf_font("font/roboto_italic.ttf", 25)


	change_game_state(STATE_TITLE)

end


function fbl_lua_loop()

	if get_game_state() == STATE_TITLE then
		tick_title()

	elseif get_game_state() == STATE_RUN_MAZE then
		tick_maze()

	elseif get_game_state() == STATE_EXPLORE then
		tick_explore()

	end

end


function fbl_lua_end()

	print("Lua end!\n")

end
