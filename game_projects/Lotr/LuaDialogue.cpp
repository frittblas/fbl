/*
*
*	Lotr
*
*	LuaDialogue.cpp
*
*	Functions to be called from the Lua dialogue system.
*
*	Hans Strömquist 2022
*
*/

#include "../../dependencies/common/lua-5.4.1/include/lua.hpp"

#include "LuaDialogue.hpp"

//#include "../../src/fbl.hpp"
//#include "Game.hpp"

//  use the correct lua environment
extern "C" {
	extern lua_State* fbl_lua_env;
}

// lotr Lua prototypes
int luaIsInDialogue(lua_State* lua_env);

bool inDialogue = true;


// register these C++ functions so they can be called from Lua.
void registerFuncsToLua()
{
	
	lua_register(fbl_lua_env, "isInDialogue", luaIsInDialogue);

}

// lotr Lua implementation
int luaIsInDialogue(lua_State* lua_env) {

	lua_pushnumber(lua_env, (lua_Number)inDialogue);

	return 1;

}
