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

// ScenEdit-class implementation

ScenEdit::ScenEdit(bool runStandAlone) {

	standAlone = runStandAlone;

	// draw everything from top left
	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);

	// set up with default values
	setup(screenWidthInTiles, screenHeightInTiles, 32);

	// setup GUI if in stand alone mode
	if (standAlone)
		setupGUI();

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

	std::cout << "Shut down ScenEdit instance!" << std::endl;

}

void ScenEdit::setup(uint32_t mapW, uint32_t mapH, uint32_t tSize) {

	// set up the editor, map and tile size
	mapWidth = mapW;
	mapHeight = mapH;
	tileSize = tSize;

	// allocate memory for the tile-list
	tile.reserve(mapWidth * mapHeight);

	// set all elements to nullptr
	for (uint32_t i = 0; i < (mapWidth * mapHeight); i++)
		tile.push_back(nullptr);

	fitTilesToScreen();

	// set bg-color (blue as default) and time of day-tint (tintColorOn set to 0 means tint deactivated)
	// these values can be set in the map save-file (map.scn)
	bgColorR = 50;
	bgColorG = 50;
	bgColorB = 150;
	tintColorR = 255;
	tintColorG = 255;
	tintColorB = 255;
	tintColorOn = 0;

	// set default values to the tile settings
	tileSettings.id = 0;	// id of the current tile to be drawn
	tileSettings.x = 0;		// x of the white cursor
	tileSettings.y = 0;
	tileSettings.textureX = 96;	// x of the tile to be drawn from texture
	tileSettings.textureY = 416;
	tileSettings.flip = 0;	// how is the tile flipped? none, horizontal, vertical, both
	tileSettings.layer = 0;	// the current layer (lower is further back)
	tileSettings.type = 0;	// type can mean different things in your game, maybe 0 = walkable etc.
	tileSettings.animated = false;	// if the tile is animated
	tileSettings.animFrames = 1;	// how many frames in total are there in the animation
	tileSettings.animSpeed = 10;	// how many frames delay between each image

}

void ScenEdit::showHelp() {

	// print help to the console
	std::cout << "Welcome to ScenEdit!" << std::endl;
	std::cout << "Control the camera with WASD." << std::endl;
	std::cout << "Control the marker with arrows or mouse." << std::endl;
	std::cout << "Draw a tile with space or left mouse button." << std::endl;
	std::cout << "Delete tile with del or right mouse button." << std::endl;
	std::cout << "Center the map with C and reset map with R." << std::endl;
	std::cout << "Copy a tile with X. Load Rwops map (text) with T." << std::endl;
	std::cout << "Load binary map with B. Sort sprites with Z." << std::endl;
	std::cout << "Show this help with H." << std::endl;
	std::cout << "Will load spritesheet.png and ui_2.png at the start." << std::endl;
	std::cout << "Will save and load map.scn (text-format) and map.scb (bin-format)" << std::endl;
	std::cout << "Tile vector size: " << tile.size() << std::endl;

}

void ScenEdit::setupGUI() {

	const int lMargin = 300; // Gui text gets drawn this far from the right

	// load textures
	fbl_load_ui_texture((char*)"ui_2.png");	// load ui texture
	fbl_load_texture((char*)"spritesheet.png");	// load sprite texture

	// load font
	fbl_load_ttf_font("edosz.ttf", 18);

	// print usage
	showHelp();

	// dark grey, filled rect with alpha as bg, draw from middle 2X size
	bgRectId = fbl_create_prim(FBL_RECT, fbl_get_screen_w() - 160, fbl_get_screen_h() / 2, 160, fbl_get_screen_h() / 2, 0, 0, true);
	fbl_set_prim_color(bgRectId, 40, 40, 40, 90);
	fbl_fix_prim_to_screen(bgRectId, true);

	// plain text ids start at 0
	fbl_create_text(255, 255, 255, 255, (char*)"Select tile:");
	fbl_set_text_xy(0, fbl_get_screen_w() - lMargin, 64);

	fbl_create_text(255, 255, 255, 255, (char*)"Animated:");
	fbl_set_text_xy(1, fbl_get_screen_w() - lMargin, 364);

	fbl_create_text(255, 255, 255, 255, (char*)"Save/Load/Exp bin:");
	fbl_set_text_xy(2, fbl_get_screen_w() - lMargin, 510);

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
	fbl_set_sprite_layer(tileSettings.id, 1);	// make sure the current drawing tile is on top, check resetMap() aswell

	// flip tile text
	flipTextId = fbl_create_text(255, 255, 255, 255, (char*)"Tile flip: %d (+)", 0);
	fbl_set_text_xy(flipTextId, fbl_get_screen_w() - lMargin, 150);

	// gui button for setting tile flip
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incFlip));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 150);

	// text showing map size
	mapWtextId = fbl_create_text(255, 255, 255, 255, (char*)"Map width: %d (+)", mapWidth);
	mapHtextId = fbl_create_text(255, 255, 255, 255, (char*)"Map height: %d (+)", mapHeight);
	fbl_set_text_xy(mapWtextId, fbl_get_screen_w() - lMargin, 190);
	fbl_set_text_xy(mapHtextId, fbl_get_screen_w() - lMargin, 230);

	// gui buttons for expanding the map size
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incMapX));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 190);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incMapY));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 230);

	// text for layer
	layerTextId = fbl_create_text(255, 255, 255, 255, (char*)"Layer: %d (-+)", 0);
	fbl_set_text_xy(layerTextId, fbl_get_screen_w() - lMargin, 284);

	// gui buttons for layer
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decLayer));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 284);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incLayer));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 284);

	// text for type
	typeTextId = fbl_create_text(255, 255, 255, 255, (char*)"Type: %d (-+)", 0);
	fbl_set_text_xy(typeTextId, fbl_get_screen_w() - lMargin, 324);

	// gui buttons for type
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decType));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 324);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incType));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 324);

	// gui checkbox for animation
	animatedBoxId = fbl_create_ui_elem(FBL_UI_CHECKBOX, 0, 32, 32, 32, toggleAnimation);
	fbl_set_ui_elem_xy(animatedBoxId, fbl_get_screen_w() - 96, 364);

	// text for animation frames
	animFramesTextId = fbl_create_text(255, 255, 255, 255, (char*)"Anim frames: %d (-+)", tileSettings.animFrames);
	fbl_set_text_xy(animFramesTextId, fbl_get_screen_w() - lMargin, 404);

	// gui buttons for anim frames
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decAnimFrames));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 404);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incAnimFrames));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 404);

	// text for animation speed
	animSpeedTextId = fbl_create_text(255, 255, 255, 255, (char*)"Anim speed: %d (-+)", tileSettings.animSpeed);
	fbl_set_text_xy(animSpeedTextId, fbl_get_screen_w() - lMargin, 444);

	// gui buttons for anim speed
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, decAnimSpeed));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 444);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, incAnimSpeed));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 48, 444);

	// gui buttons for save/load/binexp
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, saveMap));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 96, 510);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, loadMap));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 64, 510);
	guiId.push_back(fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, exportMapBin));
	fbl_set_ui_elem_xy(guiId.back(), fbl_get_screen_w() - 32, 510);

	// create the map marker rect
	mapMarkerId = fbl_create_prim(FBL_NORMAL_RECT, tileSettings.x, tileSettings.y, tileSize, tileSize, 0, 0, false);
	// rects are 2X size in fbl :)(it's because of the translate shape stuff. Just scale down 2x to make fit)
	// temporarily fixed this by adding a new shape to draw (1 after FBL_RAY, in primitives.c)
	fbl_set_prim_color(mapMarkerId, 255, 255, 255, 100);

}

void ScenEdit::fitTilesToScreen() {

	screenWidthInTiles = fbl_get_screen_w() / tileSize;
	if (fbl_get_screen_w() % tileSize > (tileSize / 2))
		screenWidthInTiles++;
	screenHeightInTiles = fbl_get_screen_h() / tileSize;
	if (fbl_get_screen_h() % tileSize > (tileSize / 2))
		screenHeightInTiles++;

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
	if (fbl_get_key_down(FBLK_D) && (fbl_get_camera_x() / tileSize) < (mapWidth - screenWidthInTiles) && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x() + tileSize, fbl_get_camera_y());
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_W) && fbl_get_camera_y() > 0 && keyAccess == 0) {
		fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - tileSize);
		keyAccess = spdFast;
	}
	if (fbl_get_key_down(FBLK_S) && (fbl_get_camera_y() / tileSize) < (mapHeight - screenHeightInTiles) && keyAccess == 0) {
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
		std::cout << "Reset camera!" << std::endl;
		keyAccess = spdSlow;
	}

	// reset map with r
	if (fbl_get_key_down(FBLK_R) && keyAccess == 0) {
		resetMap(screenWidthInTiles, screenHeightInTiles);	// reset map to fit screen
		keyAccess = spdSlow;
	}

	// sort tiles with s
	if (fbl_get_key_down(FBLK_Z) && keyAccess == 0) {
		fbl_sort_sprites(FBL_SORT_BY_LAYER);
		std::cout << "Sorted sprites!" << std::endl;
		keyAccess = spdSlow;
	}

	// load fbl text map with t
	if (fbl_get_key_down(FBLK_T) && keyAccess == 0) {
		loadMapFblText(0, 0);
		keyAccess = spdSlow;
	}

	// load fbl binary map with b
	if (fbl_get_key_down(FBLK_B) && keyAccess == 0) {
		loadMapFblBin(0, 0);
		keyAccess = spdSlow;
	}

	// help with h
	if (fbl_get_key_down(FBLK_H) && keyAccess == 0) {
		showHelp();
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
		fbl_set_sprite_flip(tmpId, tileSettings.flip);

		// add a new element to the vector
		TileData* tmpTile = new TileData();
		tile[index] = tmpTile;

		// set all values on the tile
		tile[index]->id = tmpId;
		tile[index]->x = tileSettings.x;
		tile[index]->y = tileSettings.y;
		tile[index]->textureX = tileSettings.textureX;
		tile[index]->textureY = tileSettings.textureY;
		tile[index]->flip = tileSettings.flip;
		tile[index]->layer = tileSettings.layer;
		tile[index]->type = tileSettings.type;
		tile[index]->animated = tileSettings.animated;
		tile[index]->animFrames = tileSettings.animFrames;
		tile[index]->animSpeed = tileSettings.animSpeed;

		// turn on the animation if animated == true, otherwise do nothing
		fbl_set_sprite_animation(tile[index]->id, tile[index]->animated, tile[index]->textureX, tile[index]->textureY,
			tileSize, tileSize, tile[index]->animFrames, tile[index]->animSpeed, true);

		std::cout << "Added sprite at X: " << tileSettings.x / tileSize << ", Y: " << tileSettings.y / tileSize << std::endl;
		std::cout << "Number of tiles: " << fbl_get_num_sprites() - 1 << std::endl; // -1 because of the select sprite

	}

}

void ScenEdit::copyTile() {

	// find the correct index
	int index = getIndexAtCursor();

	if (tile[index] != nullptr) {

		// copy the values from the current tile to tile settings (not the id or xy)
		tileSettings.textureX = tile[index]->textureX;
		tileSettings.textureY = tile[index]->textureY;
		tileSettings.flip = tile[index]->flip;
		tileSettings.layer = tile[index]->layer;
		tileSettings.type = tile[index]->type;
		tileSettings.animated = tile[index]->animated;
		tileSettings.animFrames = tile[index]->animFrames;
		tileSettings.animSpeed = tile[index]->animSpeed;

	}
	else {
		// copy standard values, reset
		tileSettings.textureX = 0;
		tileSettings.textureY = 0;
		tileSettings.flip = 0;
		tileSettings.layer = 0;
		tileSettings.type = 0;
		tileSettings.animated = false;
		tileSettings.animFrames = 1;
		tileSettings.animSpeed = 10;

		showTileInfo();

	}

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
		// set the flip
		fbl_update_text(flipTextId, 255, 255, 255, 255, (char*)"Tile flip: %d (+)", tileSettings.flip);
		// set the layer
		fbl_update_text(layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", tileSettings.layer);
		// set type
		fbl_update_text(typeTextId, 255, 255, 255, 255, (char*)"Type: %d (-+)", tileSettings.type);
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
		fbl_set_sprite_flip(tileSettings.id, tileSettings.flip);
		// set the flip
		fbl_update_text(flipTextId, 255, 255, 255, 255, (char*)"Tile flip: %d (+)", tile[index]->flip);
		// set the layer
		fbl_update_text(layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", tile[index]->layer);
		// set type
		fbl_update_text(typeTextId, 255, 255, 255, 255, (char*)"Type: %d (-+)", tile[index]->type);
		// set animated
		tile[index]->animated ? fbl_set_ui_elem_val(animatedBoxId, true) : fbl_set_ui_elem_val(animatedBoxId, false);
		// set the animation frames
		fbl_update_text(animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", tile[index]->animFrames);
		// set the anim speed
		fbl_update_text(animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", tile[index]->animSpeed);


		std::cout << std::endl;
		std::cout << "Tile info:" << std::endl;
		std::cout << "Flip: " << tile[index]->flip << std::endl;
		std::cout << "Layer: " << tile[index]->layer << std::endl;
		std::cout << "Type: " << tile[index]->type << std::endl;
		std::cout << "Animated: " << tile[index]->animated << std::endl;
		std::cout << "Anim frames: " << tile[index]->animFrames << std::endl;
		std::cout << "Anim speed: " << tile[index]->animSpeed << std::endl;
		std::cout << std::endl;

	}

}

void ScenEdit::resetMap(uint32_t w, uint32_t h) {

	// remove all sprites, resetting the fbl sprite id counter.
	fbl_destroy_all_sprites();

	// delete all the tile elements
	for (TileData* curTile : tile) {

		if (curTile != nullptr) {
			delete curTile;
		}

	}

	// resize the map
	mapWidth = w;
	mapHeight = h;

	// resize memory for the tile-list
	tile.resize(mapWidth * mapHeight);

	// clear the vector
	tile.clear();

	// set all elements to nullptr
	for (uint32_t i = 0; i < (mapWidth * mapHeight); i++)
		tile.push_back(nullptr);

	if (standAlone) {

		// recreate the current tile to draw, as id 0
		tileSettings.textureX = 0;
		tileSettings.textureY = 0;
		tileSettings.id = fbl_create_sprite(tileSettings.textureX, tileSettings.textureY, tileSize, tileSize, 0);
		fbl_set_sprite_xy(tileSettings.id, fbl_get_screen_w() - 96 - 16, 64 - 16); // compensate for ui center-drawing
		fbl_fix_sprite_to_screen(tileSettings.id, true);
		fbl_set_sprite_layer(tileSettings.id, 1);	// current drawing tile is on top (requires sorting with Z)

		// recreate the marMarker prim, with new size
		fbl_delete_prim(mapMarkerId);
		mapMarkerId = fbl_create_prim(FBL_NORMAL_RECT, tileSettings.x, tileSettings.y, tileSize, tileSize, 0, 0, false);
		fbl_set_prim_color(mapMarkerId, 255, 255, 255, 100);

		// set the border for the new tilesize
		fitTilesToScreen();

		// update the w and h text
		fbl_update_text(mapWtextId, 255, 255, 255, 255, (char*)"Map width: %d (+)", mapWidth);
		fbl_update_text(mapHtextId, 255, 255, 255, 255, (char*)"Map height: %d (+)", mapHeight);

		// set the map marker xy to 0
		tileSettings.x = 0;
		tileSettings.y = 0;
		fbl_set_prim_xy(mapMarkerId, tileSettings.x, tileSettings.y);

		// show the correct tile info
		showTileInfo();

	}

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
	for(int i = 0; i < 3; i++)
		fbl_set_text_active(i, showGUI);

	fbl_set_sprite_active(tileSettings.id, showGUI);

	fbl_set_text_active(mapWtextId, showGUI);
	fbl_set_text_active(mapHtextId, showGUI);

	fbl_set_text_active(flipTextId, showGUI);
	fbl_set_text_active(layerTextId, showGUI);
	fbl_set_text_active(typeTextId, showGUI);
	fbl_set_text_active(animFramesTextId, showGUI);
	fbl_set_text_active(animSpeedTextId, showGUI);

	fbl_set_ui_elem_active(animatedBoxId, showGUI);

	for (int i : guiId)
		fbl_set_ui_elem_active(i, showGUI);

}
