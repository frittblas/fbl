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
--	Beware that lua likes to start arrays on 1 and fbl starts id's (ex. fbl_create_sprite()) on 0.
--	It might be best to just use 0 as the starting point in lua aswell, to avoid going insane :)
--
--	Hans Strömquist 2017 - 2020
--
--

-- print("Loaded lua file!\n")

nums = 100

pos_x = {}
pos_y = {}
dir_x = {}
dir_y = {}
scale = {}

frams = 0

flag = 1

start_time = 0

function fbl_lua_start()


	print("Lua start!\n")
	
	fbl_set_window_title("Demo 5: Lua (edit main.lua and reload with F5!)")
	
--	fbl_set_screen_wh(960, 840)
--	fbl_set_clear_color(168, 230, 255, 255)
	
--	fbl_load_texture("assets/spritesheet.png")
--	fbl_create_sprite(0, 0, 32, 64, 0)
--	fbl_set_sprite_xy(0, 350, 150)
--	fbl_set_sprite_flip(0, 3)

	i = 0

	fbl_load_texture("p.png")
	
	for i=0, nums do
		dir_x[i] = 0
		dir_y[i] = 0
		scale[i] = 0
	end

	for i = 0, nums do
	
		fbl_create_sprite(0, 0, 64, 64, 0)
		pos_x[i] = math.random(fbl_get_screen_w())
		pos_y[i] = math.random(fbl_get_screen_h())
		fbl_set_sprite_xy(i, pos_x[i], pos_y[i])
		--fbl_set_sprite_blendmode(i, math.random(2))
		fbl_set_sprite_alpha(i, math.random(255))
		fbl_set_sprite_color(i, 255, math.random(255), math.random(255))

		dir_x[i] = 1
		if math.random(2) == 1 then dir_x[i] = -1 end

		dir_y[i] = 1
		if math.random(2) == 1 then dir_y[i] = -1 end

		scale[i] = (math.random(10) + 5) / 10.0

		fbl_set_sprite_scale(i, scale[i])

	end

	--fbl_sort_sprites()

	--fbl_set_fps_locked(false)
	--fbl_set_system_delay_ms(0)

	flag = 1
	frams = 0
	start_time = fbl_timer_get_ticks()


end

function fbl_lua_loop()


	i = 0
	r = 0
	g = 0
	b = 0


	if fbl_timer_get_ticks() > start_time + 10000 then
		if flag == 1 then
			print("10 seconds passed.\n")
			print("total frames: ", fbl_get_raw_frames_count())
			print("\nfps last 10 sec: ", frams)
			flag = 0
		end
	end

	frams = frams + 1

	for i = 0, nums do

		fbl_set_sprite_xy(i, pos_x[i], pos_y[i])
		
		pos_x[i] = pos_x[i] + dir_x[i]
		pos_y[i] = pos_y[i] + dir_y[i]

		if pos_x[i] > fbl_get_screen_w() then dir_x[i] = -1 end
		if pos_x[i] < 0 then dir_x[i] = 1 end
		if pos_y[i] > fbl_get_screen_h() then dir_y[i] = -1 end
		if pos_y[i] < 0 then dir_y[i] = 1 end

		--fbl_set_sprite_scale(i, scale[i] + 0.1)

		--r, g, b = fbl_get_sprite_color(i, r, g, b)

		r = r - 1
		g = g - 1
		b = b - 1

		--fbl_set_sprite_color(i, r, g, b)

	end


end

function fbl_lua_end()

	print("Lua end!\n")
	
end
