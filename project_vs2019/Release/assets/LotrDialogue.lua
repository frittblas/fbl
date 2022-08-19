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



function fbl_lua_start()

	make_strict()
	print("Lua dialogue system initialized!\n")
	
end

function fbl_lua_loop()

	--if in_dialogue() then
		--response_dialog()
	--end

end

function fbl_lua_end()

	print("Lua end!\n")
	
end
