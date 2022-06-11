/*
*
*	FBL ENGINE
*
*	ScenEdit.cpp
*
*	The tile editor for fbl
*
*	Hans Strömquist 2022
*
*/

#include "ScenEdit.hpp"


ScenEdit *editor;

//
// Below are the functions that are called by the Gui buttons
//

int select_sprite_left(int x, int y) {

	if(editor->drawTileX > 0)
		editor->drawTileX -= editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int select_sprite_right(int x, int y) {

	if (editor->drawTileX < 500) // change this to sprite_texture_w - tileSize
		editor->drawTileX += editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int select_sprite_up(int x, int y) {

	if (editor->drawTileY > 0)
		editor->drawTileY -= editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int select_sprite_down(int x, int y) {

	if (editor->drawTileY < 500)	// change this to sprite_texture_h - tileSize
		editor->drawTileY += editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

// ScenEdit-class implementation

ScenEdit::ScenEdit() {

    // set up the ui and map based on initializer list

	drawTileX = 96;
	drawTileY = 416;

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);

	// load textures
	fbl_load_ui_texture("ui.png");	// load ui texture
	fbl_load_texture("spritesheet_.png");	// load sprite texture

	// first UI

	// dark grey, filled rect with alpha as bg
	bgRectId = fbl_create_prim(FBL_RECT, fbl_get_screen_w() - 160, 0, 160, fbl_get_screen_h(), 0, 0, true);
	fbl_set_prim_color(bgRectId, 40, 40, 40, 90);
	fbl_fix_prim_to_screen(bgRectId, true);

	// gui buttons for selecting current tile to draw
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, select_sprite_left));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 128, 64);

	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, select_sprite_right));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 64, 64);

	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, select_sprite_up));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 32);

	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, select_sprite_down));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 96);

	// the current tile to draw
	drawTileId = fbl_create_sprite(drawTileX, drawTileY, tileSize, tileSize, 0);
	fbl_set_sprite_xy(drawTileId, fbl_get_screen_w() - 96 - 16, 64 - 16); // compensate for ui center-drawing
	fbl_fix_sprite_to_screen(drawTileId, true);

	// create the map marker rect
	mapMarkerId = fbl_create_prim(FBL_RECT, 0, 0, tileSize, tileSize, 0, 0, false);
	fbl_set_prim_color(mapMarkerId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(mapMarkerId, false);


	std::cout << "Constructor called!" << std::endl;

}

ScenEdit::~ScenEdit() {

	fbl_destroy_texture();
	fbl_destroy_all_sprites();
	fbl_destroy_all_prims();
	fbl_destroy_all_text_objects();
	fbl_destroy_ttf_font();
	fbl_destroy_ui_texture();
	fbl_destroy_all_ui_elems();
	fbl_destroy_all_emitters();
	fbl_destroy_all_sounds();
	fbl_destroy_music();
	fbl_pathf_shutdown();
	fbl_lua_shutdown();
	fbl_phys_shutdown();

	std::cout << "Destructor called!" << std::endl;

}


void ScenEdit::tick() {

	getInput();

}

void ScenEdit::getInput() {

	// toggle gui
	if (fbl_get_key_down(FBLK_TAB) && keyAccess == 0) {

		std::cout << "Toggle GUI!" << std::endl;
		showGUI = !showGUI;
		toggleGUI();
		keyAccess = 10;

	}

	// move camera
	if (fbl_get_key_down(FBLK_A) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() - tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_D) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() + tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_W) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - tileSize);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_S) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() + tileSize);
		keyAccess = spdFast;
	}

	// move marker
	if (fbl_get_key_down(FBLK_RIGHT) && keyAccess == 0) {

		mapMarkerX -= tileSize;
		fbl_set_prim_xy(mapMarkerId, mapMarkerX, mapMarkerY );
		keyAccess = spdFast;
	}

	// mouse clicks
	if (fbl_get_mouse_click(FBLMB_LEFT) && (fbl_get_mouse_x() < (fbl_get_screen_w() - 160)) && keyAccess == 0) {

		mapMarkerX = fbl_get_mouse_x();
		mapMarkerY = fbl_get_mouse_y();

		snapToGrid(mapMarkerX, mapMarkerY);

		int tmp = fbl_create_sprite(drawTileX, drawTileY, tileSize, tileSize, 0);
		fbl_set_sprite_xy(tmp, mapMarkerX + fbl_get_camera_x(), mapMarkerY + fbl_get_camera_y());

		fbl_set_prim_xy(mapMarkerId, mapMarkerX + fbl_get_camera_x(), mapMarkerY + fbl_get_camera_y());

		keyAccess = spdMed;

	}

	keyAccess--;
	if (keyAccess < 0)
		keyAccess = 0;

}

void ScenEdit::snapToGrid(int &x, int &y)
{

	while (x % tileSize != 0) x--;
	while (y % tileSize != 0) y--;

}


void ScenEdit::toggleGUI() {

	if (showGUI) {
		fbl_set_prim_active(bgRectId, true);
		fbl_set_sprite_active(drawTileId, true);

		for (int i : guiId)
			fbl_set_ui_elem_active(i, true);
	} else {
		fbl_set_prim_active(bgRectId, false);
		fbl_set_sprite_active(drawTileId, false);

		for (int i : guiId)
			fbl_set_ui_elem_active(i, false);
	}

}


//
// Below are the fbl standard functions
//


void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	fbl_engine_init(960, 540, 60);
	fbl_set_clear_color(168, 230, 255, 255);

	editor = new ScenEdit();


}

void fbl_game_loop()
{

	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9))
		fbl_set_window_mode(0);
	if (fbl_get_key_down(FBLK_F10))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN);
	if (fbl_get_key_down(FBLK_F11))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);

	editor->tick();

}

void fbl_end()
{

	delete editor;

	std::cout<<"Bye!"<<std::endl;

}

