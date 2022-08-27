--
--
--	ROBOT GAME
--
--	strictglob.lua
--
--	Makes it mandatory to declare variables.
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

----------------------------
-- You can declare new global variables with this, but just use normal assign (x = 1)
----------------------------

function declare (name, initval)
  rawset(_G, name, initval or false)
end

-- make_strict()