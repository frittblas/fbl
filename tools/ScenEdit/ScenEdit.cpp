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
#include "GuiFuncs.hpp"


ScenEdit *editor;


// ScenEdit-class implementation

ScenEdit::ScenEdit() {

	int lMargin = 300; // Gui text gets drawn this far from the right

    // set up the editor

	// draw everything from top left
	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);

	// allocate memory for the tile-list
	tile.reserve(mapWidth * mapHeight);

	// set all elements to nullptr
	for (uint32_t i = 0; i < (mapWidth * mapHeight); i++)
		tile.push_back(nullptr);

	std::cout << "Welcome to ScenEdit!" << std::endl;
	std::cout << "Control the camera with WASD." << std::endl;
	std::cout << "Control the marker with arrows or mouse." << std::endl;
	std::cout << "Draw a tile with space or left mouse button." << std::endl;
	std::cout << "Delete tile with del or right mouse button." << std::endl;
	std::cout << "Center the map with C and reset map with R." << std::endl;
	std::cout << "Will load spritesheet_.png and ui_.png at the start." << std::endl;
	std::cout << "Will save and load map.scn and export map.lua." << std::endl;
	std::cout << "Tile vector size: " << tile.size() << std::endl;

	// load textures
	fbl_load_ui_texture((char*)"ui_.png");	// load ui texture
	fbl_load_texture((char*)"spritesheet_.png");	// load sprite texture

	// load font
	fbl_load_ttf_font("edosz.ttf", 18);

	// set bg-color (blue as default) and time of day-tint (all set to 255 means tint deactivated)
	// these values can be set int the map save-file (map.sce)
	bgColorR = 50;
	bgColorG = 50;
	bgColorB = 150;
	tintColorR = 255;
	tintColorG = 255;
	tintColorB = 255;
	tintColorA = 255;

	// set default values to the tile settings
	tileSettings.id = 0;	// id of the current tile to be drawn
	tileSettings.x = 0;		// x of the white cursor
	tileSettings.y = 0;
	tileSettings.textureX = 96;	// x of the tile to be drawn from texture
	tileSettings.textureY = 416;
	tileSettings.layer = 0;	// the current layer (lower is further back)
	tileSettings.kinematic = false;	// kinematic means solid but movable, otherwise walkable
	tileSettings.animated = false;	// if the tile is animated
	tileSettings.animFrames = 1;	// how many frames in total are there in the animation
	tileSettings.animSpeed = 10;	// how many frames delay between each image

	// first UI

	// dark grey, filled rect with alpha as bg, draw from middle 2X size
	bgRectId = fbl_create_prim(FBL_RECT, fbl_get_screen_w() - 160, fbl_get_screen_h() / 2, 160, fbl_get_screen_h() / 2, 0, 0, true);
	fbl_set_prim_color(bgRectId, 40, 40, 40, 90);
	fbl_fix_prim_to_screen(bgRectId, true);

	// plain text ids start at 0
	fbl_create_text(255, 255, 255, 255, (char*)"Select tile:");
	fbl_set_text_xy(0, fbl_get_screen_w() - lMargin, 64);

	fbl_create_text(255, 255, 255, 255, (char*)"Kinematic:");
	fbl_set_text_xy(1, fbl_get_screen_w() - lMargin, 310);

	fbl_create_text(255, 255, 255, 255, (char*)"Animated:");
	fbl_set_text_xy(2, fbl_get_screen_w() - lMargin, 350);

	fbl_create_text(255, 255, 255, 255, (char*)"Save/Load/LuaExp:");
	fbl_set_text_xy(3, fbl_get_screen_w() - lMargin, 510);

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
	tileSettings.id = fbl_create_sprite(tileSettings.textureX, tileSettings.textureY, tileSize, tileSize, 0);
	fbl_set_sprite_xy(tileSettings.id, fbl_get_screen_w() - 96 - 16, 64 - 16); // compensate for ui center-drawing
	fbl_fix_sprite_to_screen(tileSettings.id, true);

	// text showing map size
	mapWtextId = fbl_create_text(255, 255, 255, 255, (char*)"Map width: %d (+)", mapWidth);
	mapHtextId = fbl_create_text(255, 255, 255, 255, (char*)"Map height: %d (+)", mapHeight);
	fbl_set_text_xy(mapWtextId, fbl_get_screen_w() - lMargin, 160);
	fbl_set_text_xy(mapHtextId, fbl_get_screen_w() - lMargin, 200);

	// gui buttons for expanding the map size
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incMapX));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 160);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incMapY));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 200);

	// text for layer
	layerTextId = fbl_create_text(255, 255, 255, 255, (char*)"Layer: %d (-+)", 0);
	fbl_set_text_xy(layerTextId, fbl_get_screen_w() - lMargin, 270);

	// gui buttons for layer
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decLayer));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 270);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incLayer));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 270);

	// gui checkbox for kinematic
	kinematicBoxId = fbl_create_ui_elem(FBL_UI_CHECKBOX, 0, 32, 32, 32, toggleKinematic);
	fbl_set_ui_elem_xy(kinematicBoxId, fbl_get_screen_w() - 96, 310);

	// gui checkbox for animation
	animatedBoxId = fbl_create_ui_elem(FBL_UI_CHECKBOX, 0, 32, 32, 32, toggleAnimation);
	fbl_set_ui_elem_xy(animatedBoxId, fbl_get_screen_w() - 96, 350);

	// text for animation frames
	animFramesTextId = fbl_create_text(255, 255, 255, 255, (char*)"Anim frames: %d (-+)", tileSettings.animFrames);
	fbl_set_text_xy(animFramesTextId, fbl_get_screen_w() - lMargin, 390);

	// gui buttons for anim frames
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decAnimFrames));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 390);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incAnimFrames));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 390);

	// text for animation speed
	animSpeedTextId = fbl_create_text(255, 255, 255, 255, (char*)"Anim speed: %d (-+)", tileSettings.animSpeed);
	fbl_set_text_xy(animSpeedTextId, fbl_get_screen_w() - lMargin, 430);

	// gui buttons for anim speed
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decAnimSpeed));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 430);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incAnimSpeed));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 430);

	// gui buttons for save/load/luaexp
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, saveMap));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 510);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, loadMap));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 64, 510);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, exportLua));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 32, 510);

	// create the map marker rect
	mapMarkerId = fbl_create_prim(FBL_RAY + 1, tileSettings.x, tileSettings.y, tileSize, tileSize, 0, 0, false);
	// rects are 2X size in fbl :)(it's because of the translate shape stuff. Just scale down 2x to make fit)
	// temporarily fixed this by adding a new shape to draw (1 after FBL_RAY, in primitives.c)
	fbl_set_prim_color(mapMarkerId, 255, 255, 255, 100);

	
	//std::cout << "Prims: " << fbl_get_num_prims() << std::endl;
	//std::cout << "UI's: " << fbl_get_num_ui_elems() << std::endl;

}

ScenEdit::~ScenEdit() {

	fbl_destroy_texture();
	fbl_destroy_all_sprites();
	fbl_destroy_all_prims();
	fbl_destroy_all_text_objects();
	fbl_destroy_ttf_font();
	fbl_destroy_ui_texture();
	fbl_destroy_all_ui_elems();
	//fbl_destroy_all_emitters();
	//fbl_destroy_all_sounds();
	//fbl_destroy_music();
	//fbl_pathf_shutdown();
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

	// move camera and restrict movement to map borders
	if (fbl_get_key_down(FBLK_A) && fbl_get_camera_x() > 0 && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() - tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_D) && (fbl_get_camera_x() / tileSize) < (mapWidth - sceenWidthInTiles) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() + tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_W) && fbl_get_camera_y() > 0 && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - tileSize);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_S) && (fbl_get_camera_y() / tileSize) < (mapHeight - sceenHeightInTiles) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() + tileSize);
		keyAccess = spdFast;
	}

	// copy tile with x
	if (fbl_get_key_down(FBLK_X) && keyAccess == 0) {
		copyTile();
		keyAccess = spdSlow;
	}

	// center camera with c
	if (fbl_get_key_down(FBLK_C) && keyAccess == 0) {
		fbl_set_camera_xy(0, 0);
		keyAccess = spdSlow;
	}

	// reset map with r
	if (fbl_get_key_down(FBLK_R) && keyAccess == 0) {
		resetMap();
		keyAccess = spdSlow;
	}

	// move marker
	if (fbl_get_key_down(FBLK_RIGHT) && tileSettings.x < ((mapWidth - 1) * tileSize) && keyAccess == 0) {
		tileSettings.x += tileSize;
		fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);
		showTileInfo();
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_LEFT) && tileSettings.x > 0 && keyAccess == 0) {
		tileSettings.x -= tileSize;
		fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);
		showTileInfo();
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_UP) && tileSettings.y > 0 && keyAccess == 0) {
		tileSettings.y -= tileSize;
		fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);
		showTileInfo();
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_DOWN) && tileSettings.y < ((mapHeight - 1) * tileSize) && keyAccess == 0) {
		tileSettings.y += tileSize;
		fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);
		showTileInfo();
		keyAccess = spdFast;
	}

	// space bar plots a tile
	if (fbl_get_key_down(FBLK_SPACE) && keyAccess == 0) {
		addTile(); // add tile at the marker
		keyAccess = spdMed;
	}

	// delete removes a tile
	if (fbl_get_key_down(FBLK_DELETE) && keyAccess == 0) {
		removeTile(); // remove tile at the marker
		keyAccess = spdMed;
	}

	processMouse(FBLMB_LEFT);
	processMouse(FBLMB_RIGHT);

	keyAccess--;
	if (keyAccess < 0)
		keyAccess = 0;

}

void ScenEdit::processMouse(int button) {

	if (fbl_get_mouse_click(button) && (fbl_get_mouse_x() < (fbl_get_screen_w() - 320)) && keyAccess == 0) {

		uint32_t tmpX = fbl_get_mouse_x() + fbl_get_camera_x();
		uint32_t tmpY = fbl_get_mouse_y() + fbl_get_camera_y();

		snapToGrid(tmpX, tmpY);

		// check if click is in bounds
		if ((tmpX < (mapWidth * tileSize)) && (tmpY < (mapHeight * tileSize))) {

			// set the marker coords to the new values
			tileSettings.x = tmpX;
			tileSettings.y = tmpY;

			// set the map marker xy where you clicked
			fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);

			// add or remove tile based on mouse button
			button == FBLMB_LEFT ? addTile() : removeTile();

			showTileInfo();

		}

		keyAccess = spdMed;
	}

}

void ScenEdit::addTile() {

	// find the correct index
	int index = getIndexAtCursor();

	if (tile[index] == nullptr) {	// only add a sprite if the vector-element is empty

		// add the sprite
		int tmpId = fbl_create_sprite(tileSettings.textureX, tileSettings.textureY, tileSize, tileSize, 0);
		fbl_set_sprite_xy(tmpId, tileSettings.x, tileSettings.y);

		// add a new element to the vector
		TileData* tmpTile = new TileData();
		tile[index] = tmpTile;

		// set all values on the tile
		tile[index]->id = tmpId;
		tile[index]->x = tileSettings.x;
		tile[index]->y = tileSettings.y;
		tile[index]->textureX = tileSettings.textureX;
		tile[index]->textureY = tileSettings.textureY;
		tile[index]->layer = tileSettings.layer;
		tile[index]->kinematic = tileSettings.kinematic;
		tile[index]->animated = tileSettings.animated;
		tile[index]->animFrames = tileSettings.animFrames;
		tile[index]->animSpeed = tileSettings.animSpeed;

		fbl_set_sprite_animation(editor->tile[index]->id, tile[index]->animated, editor->tile[index]->textureX, editor->tile[index]->textureY,
			editor->tileSize, editor->tileSize, editor->tile[index]->animFrames, editor->tile[index]->animSpeed, true);

		std::cout << "Added sprite at X: " << tileSettings.x / tileSize << ", Y: " << tileSettings.y / tileSize << std::endl;
		std::cout << "Number of tiles: " << fbl_get_num_sprites() - 1 << std::endl; // -1 because of the select sprite

	}

}

void ScenEdit::copyTile() {

	// find the correct index
	int index = getIndexAtCursor();

	// copy the values from the current tile to tile settings (not the id or xy)
	tileSettings.textureX = tile[index]->textureX;
	tileSettings.textureY = tile[index]->textureY;
	tileSettings.layer = tile[index]->layer;
	tileSettings.kinematic = tile[index]->kinematic;
	tileSettings.animated = tile[index]->animated;
	tileSettings.animFrames = tile[index]->animFrames;
	tileSettings.animSpeed = tile[index]->animSpeed;

	std::cout << "Copied current tile settings!" << std::endl;

}

void ScenEdit::removeTile() {

	int index = getIndexAtCursor();

	if (tile[index] != nullptr) {	// only remove the sprite if the vector-element is occupied

		fbl_delete_sprite(tile[index]->id);

		// delete the element in the vector
		delete tile[index];
		tile[index] = nullptr;

		std::cout << "Removed sprite at X: " << tileSettings.x / tileSize << ", Y: " << tileSettings.y / tileSize << std::endl;

		showTileInfo();

	}

}

void ScenEdit::showTileInfo() {

	int index = getIndexAtCursor();

	if (tile[index] == nullptr) {

		// if the cursor is on an empty tile show the general current tileSettings

		fbl_set_sprite_image(tileSettings.id, tileSettings.textureX, tileSettings.textureY, tileSize, tileSize, 0);
		// set the layer
		fbl_update_text(layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", tileSettings.layer);
		// set kinematic
		tileSettings.kinematic ? fbl_set_ui_elem_val(kinematicBoxId, true) : fbl_set_ui_elem_val(kinematicBoxId, false);
		// set animated
		tileSettings.animated ? fbl_set_ui_elem_val(animatedBoxId, true) : fbl_set_ui_elem_val(animatedBoxId, false);
		// set the animation frames
		fbl_update_text(animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", tileSettings.animFrames);
		// set the anim speed
		fbl_update_text(animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", tileSettings.animSpeed);

		std::cout << "Tile info: Empty slot." << std::endl;

	}
	else {

		// if the cursor is on an existing tile show the settings for that tile
		fbl_set_sprite_image(tileSettings.id, tile[index]->textureX, tile[index]->textureY, tileSize, tileSize, 0);
		// set the layer
		fbl_update_text(layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", tile[index]->layer);
		// set kinematic
		tile[index]->kinematic ? fbl_set_ui_elem_val(kinematicBoxId, true) : fbl_set_ui_elem_val(kinematicBoxId, false);
		// set animated
		tile[index]->animated ? fbl_set_ui_elem_val(animatedBoxId, true) : fbl_set_ui_elem_val(animatedBoxId, false);
		// set the animation frames
		fbl_update_text(animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", tile[index]->animFrames);
		// set the anim speed
		fbl_update_text(animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", tile[index]->animSpeed);


		std::cout << std::endl;
		std::cout << "Tile info:" << std::endl;
		std::cout << "Layer: " << tile[index]->layer << std::endl;
		std::cout << "Kinematic: " << tile[index]->kinematic << std::endl;
		std::cout << "Animated: " << tile[index]->animated << std::endl;
		std::cout << "Anim frames: " << tile[index]->animFrames << std::endl;
		std::cout << "Anim speed: " << tile[index]->animSpeed << std::endl;
		std::cout << std::endl;

	}

}

void ScenEdit::resetMap() {

	// remove all sprites, resetting the fbl sprite id counter.
	fbl_destroy_all_sprites();

	// delete all the tile elements
	for (TileData* curTile : tile) {

		if (curTile != nullptr) {
			delete curTile;
		}

	}

	// recreate the current tile to draw, as id 0
	tileSettings.textureX = 96;
	tileSettings.textureY = 416;
	tileSettings.id = fbl_create_sprite(tileSettings.textureX, tileSettings.textureY, tileSize, tileSize, 0);
	fbl_set_sprite_xy(tileSettings.id, fbl_get_screen_w() - 96 - 16, 64 - 16); // compensate for ui center-drawing
	fbl_fix_sprite_to_screen(tileSettings.id, true);

	// reset the map size to fit the screen (960x540)
	mapWidth = 30;
	mapHeight = 17;

	fbl_update_text(editor->mapWtextId, 255, 255, 255, 255, (char*)"Map width: %d (+)", editor->mapWidth);
	fbl_update_text(editor->mapHtextId, 255, 255, 255, 255, (char*)"Map height: %d (+)", editor->mapHeight);

	// resize memory for the tile-list
	tile.resize(mapWidth * mapHeight);

	// clear the vector
	tile.clear();

	// set all elements to nullptr
	for (uint32_t i = 0; i < (mapWidth * mapHeight); i++)
		tile.push_back(nullptr);

	// set the map marker xy to 0
	tileSettings.x = 0;
	tileSettings.y = 0;
	fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);

	// reset the camera
	fbl_set_camera_xy(0, 0);

	std::cout << "Reset map! Tile vector size: " << tile.size() << std::endl;

}

void ScenEdit::snapToGrid(uint32_t &x, uint32_t &y)
{

	while (x % tileSize != 0) x--;
	while (y % tileSize != 0) y--;

}


void ScenEdit::toggleGUI() {

	showGUI = !showGUI;

	fbl_set_prim_active(bgRectId, showGUI);

	// the plain text id's start at 0
	for(int i = 0; i < 4; i++)
		fbl_set_text_active(i, showGUI);

	fbl_set_sprite_active(tileSettings.id, showGUI);

	fbl_set_text_active(mapWtextId, showGUI);
	fbl_set_text_active(mapHtextId, showGUI);

	fbl_set_text_active(layerTextId, showGUI);
	fbl_set_text_active(animFramesTextId, showGUI);
	fbl_set_text_active(animSpeedTextId, showGUI);

	fbl_set_ui_elem_active(kinematicBoxId, showGUI);
	fbl_set_ui_elem_active(animatedBoxId, showGUI);

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

	fbl_set_window_title((char*) "ScenEdit 1.0");

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

	editor->resetMap();	// free tile-mem
	delete editor;

	std::cout<<"Bye!"<<std::endl;

}

