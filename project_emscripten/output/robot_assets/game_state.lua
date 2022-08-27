--
--
--	ROBOT GAME
--
--	game_state.lua
--
--	Contains global game state stuff
--
--	Hans Strömquist 2021
--
--

STATE_STARTUP =	-1

STATE_TITLE 	= 0
STATE_CUSTOMIZE = 1
STATE_RUN_MAZE	= 2
STATE_EXPLORE	= 3


-- game starts with no real state

local g_game_state = STATE_STARTUP


-- functions

-- change_game_state(new_state)
-- int get_game_state()


function change_game_state(new_state)

	if g_game_state == STATE_TITLE then

		-- deallocate title related stuff and stop title music etc.
		print("deallocate title!\n")
		
		exit_title()

	elseif g_game_state == STATE_CUSTOMIZE then
	
		-- deallocate robot customize screen related stuff and stop customize music etc.
		print("deallocate customize\n")
		
	elseif g_game_state == STATE_RUN_MAZE then
	
		-- deallocate maze run related stuff and stop main music etc.
		print("deallocate run maze\n")
		
		exit_maze()
	
	elseif g_game_state == STATE_EXPLORE then
	
		-- deallocate explore related stuff and stop exploring music etc.
		print("deallocate explore\n")

		exit_explore()
		exit_dialogue_system()
	
	end
	

	g_game_state = new_state	-- set the new state
	

	if g_game_state == STATE_TITLE then

		-- allocate title related stuff and start title music etc.
		
		print("allocate title\n")
		
		init_title()

	elseif g_game_state == STATE_CUSTOMIZE then
	
		-- allocate robot customize screen related stuff and start customize music etc.
		print("allocate customize\n")
		
	elseif g_game_state == STATE_RUN_MAZE then
	
		-- allocate maze run related stuff and start main music etc.
		print("allocate run maze\n")
		
		init_maze()
	
	elseif g_game_state == STATE_EXPLORE then
	
		-- allocate explore related stuff and start exploring music etc.
		print("allocate explore\n")

		init_explore()
		init_dialogue_system()
	
	end

end


function get_game_state()

	return g_game_state

end
