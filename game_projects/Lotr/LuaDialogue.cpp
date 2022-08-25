/*
*
*	Lotr
*
*	LuaDialogue.cpp
*
*	Functions to be called from the Lua dialogue system.
*	This file has access to gGame*.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
//#include <string>
#include "../../src/fbl.hpp"
#include "../../dependencies/common/lua-5.4.1/include/lua.hpp"
#include "Game.hpp"
#include "GameState/GameState.hpp"

//  use the correct lua environment (from fbl)
extern "C" {
	extern lua_State* fbl_lua_env;
}

// game instance
extern Game* gGame;

// id's for the dialog-box

int gSquareId, gOutlineId, gText1Id, gText2Id, gText3Id;
int gResponseYesId, gResponseNoId;
int gButtonYes, gButtonNo;


// lotr Lua prototypes
int luaIsInDialogue(lua_State* lua_env);
int luaSetState(lua_State* lua_env);
int luaDisplayDialog(lua_State* lua_env);
int luaHideDialog(lua_State* lua_env);
int luaGetResponse(lua_State* lua_env);


// register these C++ functions so they can be called from Lua.
void registerFuncsToLua()
{
	
	lua_register(fbl_lua_env, "isInDialogue", luaIsInDialogue);
	lua_register(fbl_lua_env, "setState", luaSetState);

	lua_register(fbl_lua_env, "displayDialog", luaDisplayDialog);
	lua_register(fbl_lua_env, "hideDialog", luaHideDialog);
	lua_register(fbl_lua_env, "getResponse", luaGetResponse);

}

void initLuaDialog() {

	// set position and size of the text area
	int x = gGame->LogicalResW / 2;
	int y = gGame->LogicalResH / 2 + gGame->LogicalResH / 4;
	int width = 250;
	int height = 100;

	// create wine red text area
	gSquareId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, true);
	fbl_set_prim_color(gSquareId, 200, 50, 50, 150);
	fbl_fix_prim_to_screen(gSquareId, true);

	// create white outline
	gOutlineId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, false);
	fbl_set_prim_color(gOutlineId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(gOutlineId, true);

	fbl_load_ttf_font("anirm.ttf", 15);

	// create 1st line of text
	gText1Id = fbl_create_text(255, 255, 255, 0, " ");
	fbl_set_text_align(gText1Id, FBL_ALIGN_LEFT);
	fbl_set_text_xy(gText1Id, x - 230, y - 70);

	// 2nd line
	gText2Id = fbl_create_text(255, 255, 255, 0, " ");
	fbl_set_text_align(gText2Id, FBL_ALIGN_LEFT);
	fbl_set_text_xy(gText2Id, x - 230, y - 30);

	// 3rd line
	gText3Id = fbl_create_text(255, 255, 255, 0, " ");
	fbl_set_text_align(gText3Id, FBL_ALIGN_LEFT);
	fbl_set_text_xy(gText3Id, x - 230, y + 10);

	// Yes-type response, or single "ok"
	gResponseYesId = fbl_create_text(255, 255, 255, 0, "Yes");
	fbl_set_text_align(gResponseYesId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(gResponseYesId, x - 185, y + 64);

	// No-type response
	gResponseNoId = fbl_create_text(255, 255, 255, 0, "No");
	fbl_set_text_align(gResponseNoId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(gResponseNoId, x + 30, y + 64);

	// ui yes / no and Ok buttons
	gButtonYes = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, 0);
	fbl_set_ui_elem_xy(gButtonYes, x - 215, y + 64);
	gButtonNo = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, 0);
	fbl_set_ui_elem_xy(gButtonNo, x, y + 64);

	// hide
	luaHideDialog(NULL);

	fbl_load_ttf_font("edosz.ttf", 16);
	//fbl_load_ttf_font("anirm.ttf", 15);

}

// lotr Lua implementation
int luaIsInDialogue(lua_State* lua_env) {

	int inDialogue = 0;

	if(gGame->mState->get() == GameState::StateType::Dialogue)
		inDialogue = 1;

	lua_pushnumber(lua_env, (lua_Number)inDialogue);

	return 1;

}

int luaSetState(lua_State* lua_env) {

	int state = (int)lua_tonumber(lua_env, 1);

	gGame->mState->change(*gGame, static_cast<GameState::StateType>(state));

	return 1;

}

int luaDisplayDialog(lua_State* lua_env) {

	char* text1 = (char*)lua_tostring(lua_env, 1);
	char* text2 = (char*)lua_tostring(lua_env, 2);
	char* text3 = (char*)lua_tostring(lua_env, 3);
	char* reply1 = (char*)lua_tostring(lua_env, 4);
	char* reply2 = (char*)lua_tostring(lua_env, 5);
	
	// activate the square
	fbl_set_prim_active(gSquareId, true);
	fbl_set_prim_active(gOutlineId, true);

	// update the text
	fbl_update_text(gText1Id, 255, 255, 255, 0, text1);
	fbl_update_text(gText2Id, 255, 255, 255, 0, text2);
	fbl_update_text(gText3Id, 255, 255, 255, 0, text3);

	fbl_set_text_active(gText1Id, true);
	fbl_set_text_active(gText2Id, true);
	fbl_set_text_active(gText3Id, true);

	// update the response text
	fbl_update_text(gResponseYesId, 255, 255, 255, 0, reply1);
	fbl_set_text_active(gResponseYesId, true);

	if (strlen(reply2) > 1) {
		fbl_update_text(gResponseNoId, 255, 255, 255, 0, reply2);
		fbl_set_text_active(gResponseNoId, true);
		fbl_set_ui_elem_active(gButtonNo, true);
	}

	fbl_set_ui_elem_active(gButtonYes, true);


	return 1;

}

int luaHideDialog(lua_State* lua_env) {

	fbl_set_prim_active(gSquareId, false);
	fbl_set_prim_active(gOutlineId, false);

	fbl_set_text_active(gText1Id, false);
	fbl_set_text_active(gText2Id, false);
	fbl_set_text_active(gText3Id, false);

	fbl_set_text_active(gResponseYesId, false);
	fbl_set_text_active(gResponseNoId, false);
	
	fbl_set_ui_elem_active(gButtonYes, false);
	fbl_set_ui_elem_active(gButtonNo, false);


	return 1;

}

int luaGetResponse(lua_State* lua_env) {

	int response = 0;

	if (fbl_get_ui_elem_val(gButtonYes))
		response = 1;
	if (fbl_get_ui_elem_val(gButtonNo))
		response = 2;

	lua_pushnumber(lua_env, (lua_Number)response);

	return 1;

}
