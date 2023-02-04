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


// register these C++ functions so they can be called from Lua.
void registerFuncsToLua();

// note the difference between dialogue and dialog :)
void initLuaDialog();
void destroyPrimsTextUi();

void showTalkButton(bool set);
