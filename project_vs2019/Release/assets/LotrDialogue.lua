--
--
--	LOTR
--
--	LotrDialogue.lua
--
--	Contains dialogue stuff
--
--	This will be called from your C-program if you call fbl_lua_init()
--	in the beginning of your program. First, the fbl_lua_start(),
--	then, fbl_lua_loop() will be called every frame. Last, fbl_lua_end()
--	will be called when you end the program from C to clean stuff up.
--
--	You have full access to the fbl api from here!
--  + some gameplay related lotr-functions.
--
--	Hans Strömquist 2022
--
--

-- constants from C++

Title = 0
Demo = 1
Settings = 2
Tutorial = 3

Explore = 4
Dialogue = 5
Shop = 6
Fight = 7
CardCollection = 8

-- functions that can be called

-- isInDialogue() (returns 1 or 0)
-- setState(int state)
-- displayDialog()
-- hideDialog()
-- getResponse()

-- valid responses

YES = 1
NO  = 2

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

function dispDialogW(text, reply1, reply2)

	--if not g_wait_response then
		displayDialog(text, reply1, reply2)
		g_wait_response = true
	--end

end

function setNewState(state)
	hideDialog()
	setState(state)
end

-- dialogues for the different characters

g_dialogue = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			print("Hello young masta", iter)
			displayDialog("Greetings! this is the first dialog.", "Ok", "")
		elseif iter == 2 then
			print("Hello old masta!! This is longer!", iter)
			displayDialog("Hello adventurer! May I insterest you in some reasonable items on your quest?", "Yes", "No")
		elseif iter == 3 then
			print("penultimate one", iter)
			displayDialog("if this works its fine", "Ok", "")
		else
			print("last one", iter)
			displayDialog("if THIS works its nice, last message this is, im sure of it", "Ok", "")
		end

		debug_console(iter)
		iter = iter + 1
		
		coroutine.yield()
	end
	end)
	
	g_dialogue1 = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				dispDialogW("Greetings! this is the first dialog.", "Ok", "")
			elseif g_wait_response then
				if getResponse() == NO then
					g_wait_response = false
					--iter = iter + 1	-- advance dialogue
					setNewState(Explore)
				end
			end
			
		elseif iter == 2 then
			print("Hello old masta!! This is longer!", iter)
			displayDialog("Hello adventurer! May I insterest you in some reasonable items on your quest?", "Yes", "No")
		elseif iter == 3 then
			print("penultimate one", iter)
			displayDialog("if this works its fine", "Ok", "")
		else
			print("last one", iter)
			displayDialog("if THIS works its nice, last message this is, im sure of it", "Ok", "")
		end

		--debug_console(iter)
		
		coroutine.yield()
		
	end
	end)

function fbl_lua_start()

	make_strict()
	g_current_dialogue = g_dialogue1
	print("Lua dialogue system initialized!\n")
	
end

function fbl_lua_loop()

	if isInDialogue() > 0 then
	
		coroutine.resume(g_current_dialogue)
		
	end

end

function fbl_lua_end()

	print("Lua dialogue system ended!\n")
	
end
