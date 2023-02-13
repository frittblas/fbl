/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	LuaDialogue.cpp
*
*	Functions to be called from the Lua dialogue system.
*	This file has access to gGame*.
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "../../dependencies/common/lua-5.4.1/include/lua.hpp"
#include "Game.hpp"
#include "GameState/GameState.hpp"
#include "LuaDialogue.hpp"

//  use the correct lua environment (from fbl)
extern "C" {
	extern lua_State* fbl_lua_env;
}

// game instance (only externed here)
extern Game* gGame;

// id's for the dialog-box f for filescope
int fDiaBgSquareId, fDiaBgOutlineId, fDiaText1Id, fDiaText2Id, fDiaText3Id;
int fDiaResponseYesId, fDiaResponseNoId;
int fDiaButtonYes, fDiaButtonNo;
int fDiaTextTalk;

int gButtonTalk; // externed in DialogueTrigSystem
int gCurrentDialogueId = 0; // id of the current dialogue to start, externed in DialogueTrigSystem


// Ca2 Lua prototypes
int luaGetState(lua_State* lua_env);
int luaSetState(lua_State* lua_env);
int luaDisplayDialog(lua_State* lua_env);
int luaHideDialog(lua_State* lua_env);
int luaGetResponse(lua_State* lua_env);
int luaGetCurrentDialogueId(lua_State* lua_env);

// register these C++ functions so they can be called from Lua.
void registerFuncsToLua()
{
	
	lua_register(fbl_lua_env, "getState", luaGetState);
	lua_register(fbl_lua_env, "setState", luaSetState);

	lua_register(fbl_lua_env, "displayDialog", luaDisplayDialog);
	lua_register(fbl_lua_env, "hideDialog", luaHideDialog);
	lua_register(fbl_lua_env, "getResponse", luaGetResponse);

	lua_register(fbl_lua_env, "getCurrentDialogueId", luaGetCurrentDialogueId);

}

void initLuaDialog() {

	// set position and size of the text area
	int x = Game::DeviceResW / 2;
	int y = Game::DeviceResH / 2 + Game::DeviceResH / 4;
	int width = 250;
	int height = 100;

	// create wine red text area
	fDiaBgSquareId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, true);
	fbl_set_prim_color(fDiaBgSquareId, 200, 50, 50, 150);
	fbl_fix_prim_to_screen(fDiaBgSquareId, true);

	// create white outline
	fDiaBgOutlineId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, false);
	fbl_set_prim_color(fDiaBgOutlineId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(fDiaBgOutlineId, true);

	fbl_load_ttf_font("font/garamond.ttf", 22);

	// create 1st line of text
	fDiaText1Id = fbl_create_text(255, 255, 255, 0, (char*)" ");
	fbl_set_text_align(fDiaText1Id, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fDiaText1Id, x - 230, y - 70);

	// 2nd line
	fDiaText2Id = fbl_create_text(255, 255, 255, 0, (char*)" ");
	fbl_set_text_align(fDiaText2Id, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fDiaText2Id, x - 230, y - 30);

	// 3rd line
	fDiaText3Id = fbl_create_text(255, 255, 255, 0, (char*)" ");
	fbl_set_text_align(fDiaText3Id, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fDiaText3Id, x - 230, y + 10);

	// Yes-type response, or single "ok"
	fDiaResponseYesId = fbl_create_text(255, 255, 255, 0, (char*)"Yes");
	fbl_set_text_align(fDiaResponseYesId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fDiaResponseYesId, x - 185, y + 64);

	// No-type response
	fDiaResponseNoId = fbl_create_text(255, 255, 255, 0, (char*)"No");
	fbl_set_text_align(fDiaResponseNoId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fDiaResponseNoId, x + 30, y + 64);

	// ui yes / no and Ok buttons
	fDiaButtonYes = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fDiaButtonYes, x - 215, y + 64);
	fbl_set_ui_elem_access(fDiaButtonYes, 60);
	fDiaButtonNo = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fDiaButtonNo, x, y + 64);
	fbl_set_ui_elem_access(fDiaButtonNo, 60);

	// the "Talk" text and button
	fDiaTextTalk = fbl_create_text(255, 255, 255, 0, (char*)"Talk");
	fbl_set_text_align(fDiaTextTalk, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fDiaTextTalk, x, 32);

	gButtonTalk = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(gButtonTalk, x - 32, 32);


	// hide
	showTalkButton(false);
	luaHideDialog(NULL);

}

void destroyAllGfx() {

	fbl_destroy_all_sprites();
	fbl_destroy_all_prims();
	fbl_destroy_all_text_objects();
	fbl_destroy_all_ui_elems();
	fbl_destroy_all_emitters();

}

void showTalkButton(bool set) {

	fbl_set_text_active(fDiaTextTalk, set);
	fbl_set_ui_elem_active(gButtonTalk, set);

}

//
//	Here follows the high level gameplay functions that you can call from the Lua dialogue system.
//

int luaGetState(lua_State* lua_env) {

	
	lua_pushnumber(lua_env, (lua_Number)static_cast<int>(gGame->mState->get()));

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
	fbl_set_prim_active(fDiaBgSquareId, true);
	fbl_set_prim_active(fDiaBgOutlineId, true);

	// update the text
	fbl_update_text(fDiaText1Id, 255, 255, 255, 0, text1);
	fbl_update_text(fDiaText2Id, 255, 255, 255, 0, text2);
	fbl_update_text(fDiaText3Id, 255, 255, 255, 0, text3);

	fbl_set_text_active(fDiaText1Id, true);
	fbl_set_text_active(fDiaText2Id, true);
	fbl_set_text_active(fDiaText3Id, true);

	// update the response text
	fbl_update_text(fDiaResponseYesId, 255, 255, 255, 0, reply1);
	fbl_set_text_active(fDiaResponseYesId, true);

	// if you want a single ? or something, as a reply option, write a space after (in Lua) so it gets picked up by this.
	if (strlen(reply2) > 1) {
		fbl_update_text(fDiaResponseNoId, 255, 255, 255, 0, reply2);
		fbl_set_text_active(fDiaResponseNoId, true);
		fbl_set_ui_elem_active(fDiaButtonNo, true);
	}

	fbl_set_ui_elem_active(fDiaButtonYes, true);


	return 1;

}

int luaHideDialog(lua_State* lua_env) {

	fbl_set_prim_active(fDiaBgSquareId, false);
	fbl_set_prim_active(fDiaBgOutlineId, false);

	fbl_set_text_active(fDiaText1Id, false);
	fbl_set_text_active(fDiaText2Id, false);
	fbl_set_text_active(fDiaText3Id, false);

	fbl_set_text_active(fDiaResponseYesId, false);
	fbl_set_text_active(fDiaResponseNoId, false);
	
	fbl_set_ui_elem_active(fDiaButtonYes, false);
	fbl_set_ui_elem_active(fDiaButtonNo, false);


	return 1;

}

int luaGetResponse(lua_State* lua_env) {

	int response = 0;

	if (fbl_get_ui_elem_val(fDiaButtonYes))
		response = 1;
	else if (fbl_get_ui_elem_active(fDiaButtonNo) && fbl_get_ui_elem_val(fDiaButtonNo))
		response = 2;

	lua_pushnumber(lua_env, (lua_Number)response);

	return 1;

}

int luaGetCurrentDialogueId(lua_State* lua_env) {

	lua_pushnumber(lua_env, gCurrentDialogueId);

	return 1;

}
