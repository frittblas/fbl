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

int selectSpriteLeft(int x, int y) {

	if(editor->drawTileX > 0)
		editor->drawTileX -= editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteRight(int x, int y) {

	if (editor->drawTileX < 500) // change this to sprite_texture_w - tileSize
		editor->drawTileX += editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteUp(int x, int y) {

	if (editor->drawTileY > 0)
		editor->drawTileY -= editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteDown(int x, int y) {

	if (editor->drawTileY < 500)	// change this to sprite_texture_h - tileSize
		editor->drawTileY += editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

// 

int incMapX(int x, int y) {

	printf("inc map x\n");

	editor->mapWidth++;

	fbl_update_text(editor->mapSizeXTextId, 255, 255, 255, 255, "Map size x: %d", editor->mapWidth);

	return 0;

}

int incMapY(int x, int y) {

	printf("inc map y\n");

	editor->mapHeight++;

	fbl_update_text(editor->mapSizeYTextId, 255, 255, 255, 255, "Map size y: %d", editor->mapHeight);

	return 0;

}

// ScenEdit-class implementation

ScenEdit::ScenEdit() {

    // set up the ui and map based on initializer list

	drawTileX = 96;
	drawTileY = 416;

	// draw everything from top left
	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);

	// allocate memory for the Tile-list
	tile.reserve(mapWidth * mapHeight);

	// load textures
	fbl_load_ui_texture("ui.png");	// load ui texture
	fbl_load_texture("spritesheet_.png");	// load sprite texture

	// load font
	fbl_load_ttf_font("edosz.ttf", 18);

	// first UI

	// dark grey, filled rect with alpha as bg, draw from middle 2X size
	bgRectId = fbl_create_prim(FBL_RECT, fbl_get_screen_w() - 160, fbl_get_screen_h() / 2, 160, fbl_get_screen_h() / 2, 0, 0, true);
	fbl_set_prim_color(bgRectId, 40, 40, 40, 90);
	fbl_fix_prim_to_screen(bgRectId, true);

	// plain text ids start at 0
	fbl_create_text(0, 0, 0, 255, "Select tile:");
	fbl_set_text_xy(0, fbl_get_screen_w() - 300, 64);

	// gui buttons for selecting current tile to draw
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, selectSpriteLeft));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 128, 64);

	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, selectSpriteRight));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 64, 64);

	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, selectSpriteUp));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 32);

	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, selectSpriteDown));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 96);

	// the current tile to draw
	drawTileId = fbl_create_sprite(drawTileX, drawTileY, tileSize, tileSize, 0);
	fbl_set_sprite_xy(drawTileId, fbl_get_screen_w() - 96 - 16, 64 - 16); // compensate for ui center-drawing
	fbl_fix_sprite_to_screen(drawTileId, true);

	// text showing map size
	mapSizeXTextId = fbl_create_text(0, 0, 0, 255, "Map size x: %d (+)", mapWidth);
	mapSizeYTextId = fbl_create_text(0, 0, 0, 255, "Map size y: %d (+)", mapHeight);
	fbl_set_text_xy(mapSizeXTextId, fbl_get_screen_w() - 300, 160);
	fbl_set_text_xy(mapSizeYTextId, fbl_get_screen_w() - 300, 200);

	// gui buttons for expanding the map size
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incMapX));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 160);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incMapY));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 200);

	// create the map marker rect
	mapMarkerX = 0;
	mapMarkerY = 0;
	mapMarkerId = fbl_create_prim(FBL_RAY + 1, mapMarkerX, mapMarkerY, tileSize, tileSize, 0, 0, false);
	// rects are 2X size in fbl :) pls fix (it's because of the translate shape stuff just scale that down 2x)
	// temporarily fixed this by adding a new shape to draw (after FBL_RAY, in primitives.c)
	fbl_set_prim_color(mapMarkerId, 255, 255, 255, 100);
	fbl_fix_prim_to_screen(mapMarkerId, true);


	std::cout << "Prims: " << fbl_get_num_prims() << std::endl;
	std::cout << "UI's: " << fbl_get_num_ui_elems() << std::endl;

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

}


void ScenEdit::tick() {

	getInput();

}

void ScenEdit::getInput() {

	// toggle gui
	if (fbl_get_key_down(FBLK_TAB) && keyAccess == 0) {

		std::cout << "Toggle GUI!" << std::endl;
		
		toggleGUI();
		keyAccess = spdMed;

	}

	// move camera
	if (fbl_get_key_down(FBLK_A) && fbl_get_camera_x() > 0 && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() - tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_D) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() + tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_W) && fbl_get_camera_y() > 0 && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - tileSize);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_S) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() + tileSize);
		keyAccess = spdFast;
	}

	// move marker
	if (fbl_get_key_down(FBLK_RIGHT) && mapMarkerX < (mapWidth * tileSize) && keyAccess == 0) {
		mapMarkerX += tileSize;
		fbl_set_prim_xy(mapMarkerId, mapMarkerX, mapMarkerY);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_LEFT) && mapMarkerX > 0 && keyAccess == 0) {
		mapMarkerX -= tileSize;
		fbl_set_prim_xy(mapMarkerId, mapMarkerX, mapMarkerY);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_UP) && mapMarkerY > 0 && keyAccess == 0) {
		mapMarkerY -= tileSize;
		fbl_set_prim_xy(mapMarkerId, mapMarkerX, mapMarkerY);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_DOWN) && mapMarkerY < (mapHeight * tileSize) && keyAccess == 0) {
		mapMarkerY += tileSize;
		fbl_set_prim_xy(mapMarkerId, mapMarkerX, mapMarkerY);
		keyAccess = spdFast;
	}

	// space bar plots a tile
	if (fbl_get_key_down(FBLK_SPACE) && keyAccess == 0) {
		
		if (tile.at(mapMarkerX + mapWidth * mapMarkerY).id > 0) {

			printf("mapMarkerX %d, Y: %d\n", mapMarkerX / tileSize, mapMarkerY / tileSize);

			int tmp = fbl_create_sprite(drawTileX, drawTileY, tileSize, tileSize, 0);
			fbl_set_sprite_xy(tmp, mapMarkerX + fbl_get_camera_x(), mapMarkerY + fbl_get_camera_y());

		}

			keyAccess = spdMed;
	}


	// mouse clicks
	if (fbl_get_mouse_click(FBLMB_LEFT) && (fbl_get_mouse_x() < (fbl_get_screen_w() - 300)) && keyAccess == 0) {

		mapMarkerX = fbl_get_mouse_x();
		mapMarkerY = fbl_get_mouse_y();

		snapToGrid(mapMarkerX, mapMarkerY);

		int tmp = fbl_create_sprite(drawTileX, drawTileY, tileSize, tileSize, 0);
		fbl_set_sprite_xy(tmp, mapMarkerX + fbl_get_camera_x(), mapMarkerY + fbl_get_camera_y());

		// set the map marker xy where you clicked
		fbl_set_prim_xy(mapMarkerId, mapMarkerX + fbl_get_camera_x(), mapMarkerY + fbl_get_camera_y());

		std::cout << "click x: " << fbl_get_mouse_x() << std::endl;


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

	showGUI = !showGUI;

	fbl_set_prim_active(bgRectId, showGUI);

	// the plain text id's start at 0
	fbl_set_text_active(0, showGUI);

	fbl_set_sprite_active(drawTileId, showGUI);

	fbl_set_text_active(mapSizeXTextId, showGUI);
	fbl_set_text_active(mapSizeYTextId, showGUI);

	for (int i : guiId)
		fbl_set_ui_elem_active(i, showGUI);

}


//
// Below are the fbl standard functions
//


void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	fbl_engine_init(960, 540, 60);
	fbl_set_clear_color(50, 50, 150, 255);

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

