/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	LuaDialogue.hpp
*
*	Header for the Lua dialogue system.
*
*	Hans Strömquist 2022
*
*/

struct lua_State;

// register these C++ functions so they can be called from Lua.
void registerFuncsToLua();

// note the difference between dialogue and dialog :)
void initLuaDialog();
int  luaHideDialog(lua_State* lua_env);
void setLuaWaitResponse();
void destroyAllGfx();

void showTalkButton(bool set);
