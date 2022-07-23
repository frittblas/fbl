/*
*
*	FBL ENGINE
*
*	GuiFuncs.cpp
*
*	The fbl Gui-elements call functions through pointers when clicked.
*   These are the functions, main  file
*
*	Hans Strömquist 2022
*
*/

#include "ScenEdit.hpp"
#include "Disk.hpp"

extern ScenEdit *editor;

// return the index for the tile vector based on the cursor coordinates.
int getIndexAtCursor() {

	return (editor->tileSettings.x / editor->tileSize) + editor->mapWidth * (editor->tileSettings.y / editor->tileSize);

}

//
// Below are the functions that are called by the Gui buttons
//

int selectSpriteLeft(int x, int y) {

	if (editor->tileSettings.textureX > 0)
		editor->tileSettings.textureX -= editor->tileSize;

	fbl_set_sprite_image(editor->tileSettings.id, editor->tileSettings.textureX, editor->tileSettings.textureY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteRight(int x, int y) {

	if (editor->tileSettings.textureX < (fbl_get_texture_w() - editor->tileSize))
		editor->tileSettings.textureX += editor->tileSize;

	fbl_set_sprite_image(editor->tileSettings.id, editor->tileSettings.textureX, editor->tileSettings.textureY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteUp(int x, int y) {

	if (editor->tileSettings.textureY > 0)
		editor->tileSettings.textureY -= editor->tileSize;

	fbl_set_sprite_image(editor->tileSettings.id, editor->tileSettings.textureX, editor->tileSettings.textureY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteDown(int x, int y) {

	if (editor->tileSettings.textureY < (fbl_get_texture_h() - editor->tileSize))
		editor->tileSettings.textureY += editor->tileSize;

	fbl_set_sprite_image(editor->tileSettings.id, editor->tileSettings.textureX, editor->tileSettings.textureY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

// these are called by gui buttons to increase tha map size

int incMapX(int x, int y) {

	std::cout << "inc map width" << std::endl;
	editor->mapWidth++;
	fbl_update_text(editor->mapWtextId, 255, 255, 255, 255, (char*)"Map width: %d (+)", editor->mapWidth);

	return 0;

}

int incMapY(int x, int y) {

	// NOTE: actually change the size of the vector also :)

	std::cout << "inc map height" << std::endl;
	editor->mapHeight++;
	fbl_update_text(editor->mapHtextId, 255, 255, 255, 255, (char*)"Map height: %d (+)", editor->mapHeight);

	return 0;

}

// the following functions manipulate individual tiles via gui-buttons

int incLayer(int x, int y) {


	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		editor->tile[index]->layer++;
		fbl_update_text(editor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", editor->tile[index]->layer);
	}
	else {
		editor->tileSettings.layer++;
		fbl_update_text(editor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", editor->tileSettings.layer);
	}

	return 0;

}

int decLayer(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		if (editor->tile[index]->layer > 0) {
			editor->tile[index]->layer--;
			fbl_update_text(editor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", editor->tile[index]->layer);
		}
	}
	else {
		if (editor->tileSettings.layer > 0) {
			editor->tileSettings.layer--;
			fbl_update_text(editor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", editor->tileSettings.layer);
		}
	}

	return 0;

}

int toggleKinematic(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		editor->tile[index]->kinematic = !editor->tile[index]->kinematic;
		std::cout << "Tile is kinematic: " << editor->tile[index]->kinematic << std::endl;
	}
	else {
		editor->tileSettings.kinematic = !editor->tileSettings.kinematic;
	}

	return 0;

}

int toggleAnimation(int x, int y) {


	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {

		editor->tile[index]->animated = !editor->tile[index]->animated;
		fbl_set_sprite_animation(editor->tile[index]->id, editor->tile[index]->animated, editor->tile[index]->textureX, editor->tile[index]->textureY,
			editor->tileSize, editor->tileSize, editor->tile[index]->animFrames, editor->tile[index]->animSpeed, true);

		std::cout << "Tile is animated: " << editor->tile[index]->animated << std::endl;

	}
	else {
		editor->tileSettings.animated = !editor->tileSettings.animated;
	}

	return 0;

}

int incAnimFrames(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		editor->tile[index]->animFrames++;
		fbl_update_text(editor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", editor->tile[index]->animFrames);
	}
	else {
		editor->tileSettings.animFrames++;
		fbl_update_text(editor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", editor->tileSettings.animFrames);
	}

	return 0;

}

int decAnimFrames(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		if (editor->tile[index]->animFrames > 1) {
			editor->tile[index]->animFrames--;
			fbl_update_text(editor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", editor->tile[index]->animFrames);
		}
	}
	else {
		if (editor->tileSettings.animFrames > 1) {
			editor->tileSettings.animFrames--;
			fbl_update_text(editor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", editor->tileSettings.animFrames);
		}
	}

	return 0;

}

int incAnimSpeed(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		editor->tile[index]->animSpeed++;
		fbl_update_text(editor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", editor->tile[index]->animSpeed);
	}
	else {
		editor->tileSettings.animSpeed++;
		fbl_update_text(editor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", editor->tileSettings.animSpeed);
	}

	return 0;

}

int decAnimSpeed(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		if (editor->tile[index]->animSpeed > 1) {
			editor->tile[index]->animSpeed--;
			fbl_update_text(editor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", editor->tile[index]->animSpeed);
		}
	}
	else {
		if (editor->tileSettings.animSpeed > 1) {
			editor->tileSettings.animSpeed--;
			fbl_update_text(editor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", editor->tileSettings.animSpeed);
		}
	}

	return 0;

}

int saveMap(int x, int y) {

	bool success = Disk::getInstance().saveMap(*editor, "assets/map.scn");

	if (success)
		std::cout << "Saved map to assets/map.scn" << std::endl;
	else
		std::cout << "Error saving map!" << std::endl;

	return 0;

}

int loadMap(int x, int y) {

	bool success = Disk::getInstance().loadMap(*editor, "assets/map.scn");

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

	return 0;

}

int exportLua(int x, int y) {

	bool success = Disk::getInstance().exportMapLua(*editor, "assets/map.lua");

	if (success) {
		std::cout << "Exported map to assets/map.lua" << std::endl;
	}
	else
		std::cout << "Error exporting map!" << std::endl;

	return 0;

}
