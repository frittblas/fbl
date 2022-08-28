--
--
--	LOTR
--
--	LotrDialogue.lua
--
--	Contains dialogue stuff for lotr
--
--	This will be called from your C/C++-program if you call fbl_lua_init()
--	in the beginning of your program. First, the fbl_lua_start(),
--	then, fbl_lua_loop() will be called every frame. Last, fbl_lua_end()
--	will be called when you end the program from C/C++ to clean stuff up.
--
--	You have full access to the fbl api from here!
--  + some special gameplay related lotr-functions.
--
--	Hans Str�mquist 2022
--
--

-- constants

Stay = -1
Title = 0
Demo = 1
Settings = 2
Tutorial = 3

Explore = 4
Dialogue = 5
Shop = 6
Fight = 7
CardCollection = 8

-- valid responses

OK	= 1
YES = 1
NO  = 2

-- functions that can be called

-- getState() (returns the above states, except Stay)
-- setState(int state)
-- displayDialog()
-- hideDialog()
-- getResponse() (returns 0 or 1, 2 (OK, YES, NO))

-- globals

local g_current_dialogue = nil
local g_wait_response = false

----------------------------
-- The following two function will force you to declare all variables beforehand (great)
-- functions don't need this
----------------------------

function make_strict()
	setmetatable(_G, {
	  __newindex = function (_, n)
		error("attempt to write to undeclared variable "..n, 2)
	  end,
	  __index = function (_, n)
		error("attempt to read undeclared variable "..n, 2)
	  end,
	})
end

-- print some useful debug info

function debug_console(iter)

	print("iter = ", iter)

end

-- display dialog and set wait for response-flag
function disp_dw(text1, text2, text3, reply1, reply2)

	displayDialog(text1, text2, text3, reply1, reply2)
	g_wait_response = true

end

-- hide dialog and set state
function set_state(state)
	hideDialog()
	setState(state)
end

--advance dialogue
function advance(state, iter)
	g_wait_response = false -- not waiting for response anymore
	hideDialog()
	if state ~= Stay then	-- only change state if not Staying (in dialogue)
		setState(state)
	end
	--print("reached!")
	--debug_console(iter)
	return iter
end

--
-- dialogues for the different characters
--
-- If you don't want anything on lines 2 and 3, just leave one empy space (" ")! Nothing else.
-- If you only want one character as reply 2, type "? ", so it gets picked up correctly.

	g_dialogue1 = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Greetings! this is the first dialog.", "Second line.", "Third glorious line of exquisit writing..", "Ok..", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, iter + 1) -- advance to next dialog and go to Explore state
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("If you answer no, you will be sent back", "to first dialog!", " ", "That's arosta..", "That's arostarous!")
			elseif getResponse() == NO then
				iter = advance(Stay, 1)
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1) -- advance to next dialog and don't change state
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("..", "What's your..", "..favourite colour?", "Green", "Blue")
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1)
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 2) -- advance 2 dialog steps and stay
			end
		elseif iter == 4 then
			if not g_wait_response then
				disp_dw("You chose Green!", " ", " ", "Ok great.", "Can I go?")
			elseif getResponse() == YES or getResponse() == NO then
				iter = advance(Explore, 1)
			end
		else
			if not g_wait_response then
				disp_dw("You chose Blue!", " ", " ", "Ok", "")
			elseif getResponse() == OK then
				iter = advance(Explore, 1) -- reset dialogue and go to Explore state
			end
		end

		coroutine.yield()
		
	end
	end)

function fbl_lua_start()

	make_strict()	-- have to declare variables beforehand
	g_current_dialogue = g_dialogue1	-- set curreent dialogue to something relevant
	print("Lua dialogue system initialized!")
	
end

function fbl_lua_loop()

	if getState() == Dialogue then
	
		-- first find the correct dialogue, somehow :)
		-- check getCurrentDoalogue() that returns an id
		-- then set g_current_dialogue to the correct one.
	
		-- then resume it!
		coroutine.resume(g_current_dialogue)
		
	elseif getState() == Title then
		-- reset the iter-variables in all the coroutines once!
	end

end

function fbl_lua_end()

	print("Lua dialogue system ended!\n")
	
end