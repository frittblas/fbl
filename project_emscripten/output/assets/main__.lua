--
--
--	FBL ENGINE
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
--	Hans Strömquist 2017 - 2019
--
--

-- print("Loaded lua file!\n")

function fbl_lua_start()

	print("Lua start good!\n")
	
--	fbl_engine_init(960, 540, 30)
--	fbl_set_clear_color(168, 230, 255, 255)
	
--	fbl_load_texture("assets/spritesheet.png")
--	fbl_create_sprite(0, 0, 32, 64, 0)
--	fbl_set_sprite_xy(0, 350, 150)
--	fbl_set_sprite_flip(0, 3)

end

function fbl_lua_loop()

	print("Lua loop good!\n")

end

function fbl_lua_end()

	print("Lua end!\n")

end
