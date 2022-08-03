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

extern ScenEdit *gEditor;

// return the index for the tile vector based on the cursor coordinates.
int getIndexAtCursor() {

	return (gEditor->tileSettings.x / gEditor->tileSize) + gEditor->mapWidth * (gEditor->tileSettings.y / gEditor->tileSize);

}

//
// Below are the functions that are called by the Gui buttons
//

int selectSpriteLeft(int x, int y) {

	if (gEditor->tileSettings.textureX > 0)
		gEditor->tileSettings.textureX -= gEditor->tileSize;

	fbl_set_sprite_image(gEditor->tileSettings.id, gEditor->tileSettings.textureX, gEditor->tileSettings.textureY, gEditor->tileSize, gEditor->tileSize, 0);

	return 0;

}

int selectSpriteRight(int x, int y) {

	if (gEditor->tileSettings.textureX < (fbl_get_texture_w() - gEditor->tileSize))
		gEditor->tileSettings.textureX += gEditor->tileSize;

	fbl_set_sprite_image(gEditor->tileSettings.id, gEditor->tileSettings.textureX, gEditor->tileSettings.textureY, gEditor->tileSize, gEditor->tileSize, 0);

	return 0;

}

int selectSpriteUp(int x, int y) {

	if (gEditor->tileSettings.textureY > 0)
		gEditor->tileSettings.textureY -= gEditor->tileSize;

	fbl_set_sprite_image(gEditor->tileSettings.id, gEditor->tileSettings.textureX, gEditor->tileSettings.textureY, gEditor->tileSize, gEditor->tileSize, 0);

	return 0;

}

int selectSpriteDown(int x, int y) {

	if (gEditor->tileSettings.textureY < (fbl_get_texture_h() - gEditor->tileSize))
		gEditor->tileSettings.textureY += gEditor->tileSize;

	fbl_set_sprite_image(gEditor->tileSettings.id, gEditor->tileSettings.textureX, gEditor->tileSettings.textureY, gEditor->tileSize, gEditor->tileSize, 0);

	return 0;

}

// these are called by gui buttons to increase tha map size

int incMapX(int x, int y) {

	gEditor->mapWidth++;

	gEditor->tile.resize(gEditor->mapWidth * gEditor->mapHeight);

	int lastIndex = gEditor->tile.size();

	std::cout << "lastIndex = " << lastIndex << std::endl;

	// set all the new elements to nullptr
	for (uint32_t i = lastIndex; i < (gEditor->mapWidth * gEditor->mapHeight); i++)
		gEditor->tile.push_back(nullptr);

	// update text object and write to the console
	fbl_update_text(gEditor->mapWtextId, 255, 255, 255, 255, (char*)"Map width: %d (+)", gEditor->mapWidth);
	std::cout << "inc map width" << std::endl;

	return 0;

}

int incMapY(int x, int y) {

	gEditor->mapHeight++;

	gEditor->tile.resize(gEditor->mapWidth * gEditor->mapHeight);

	int lastIndex = gEditor->tile.size();

	std::cout << "lastIndex = " << lastIndex << std::endl;

	// set all the new elements to nullptr
	for (uint32_t i = lastIndex; i < (gEditor->mapWidth * gEditor->mapHeight); i++)
		gEditor->tile.push_back(nullptr);

	fbl_update_text(gEditor->mapHtextId, 255, 255, 255, 255, (char*)"Map height: %d (+)", gEditor->mapHeight);
	std::cout << "inc map height" << std::endl;

	return 0;

}

// the following functions manipulate individual tiles via gui-buttons

int incLayer(int x, int y) {


	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		gEditor->tile[index]->layer++;
		fbl_update_text(gEditor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", gEditor->tile[index]->layer);
	}
	else {
		gEditor->tileSettings.layer++;
		fbl_update_text(gEditor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", gEditor->tileSettings.layer);
	}

	return 0;

}

int decLayer(int x, int y) {

	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		if (gEditor->tile[index]->layer > 0) {
			gEditor->tile[index]->layer--;
			fbl_update_text(gEditor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", gEditor->tile[index]->layer);
		}
	}
	else {
		if (gEditor->tileSettings.layer > 0) {
			gEditor->tileSettings.layer--;
			fbl_update_text(gEditor->layerTextId, 255, 255, 255, 255, (char*)"Layer: %d (-+)", gEditor->tileSettings.layer);
		}
	}

	return 0;

}

int toggleKinematic(int x, int y) {

	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		gEditor->tile[index]->kinematic = !gEditor->tile[index]->kinematic;
		std::cout << "Tile is kinematic: " << gEditor->tile[index]->kinematic << std::endl;
	}
	else {
		gEditor->tileSettings.kinematic = !gEditor->tileSettings.kinematic;
	}

	return 0;

}

int toggleAnimation(int x, int y) {


	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {

		gEditor->tile[index]->animated = !gEditor->tile[index]->animated;
		fbl_set_sprite_animation(gEditor->tile[index]->id, gEditor->tile[index]->animated, gEditor->tile[index]->textureX, gEditor->tile[index]->textureY,
			gEditor->tileSize, gEditor->tileSize, gEditor->tile[index]->animFrames, gEditor->tile[index]->animSpeed, true);

		std::cout << "Tile is animated: " << gEditor->tile[index]->animated << std::endl;

	}
	else {
		gEditor->tileSettings.animated = !gEditor->tileSettings.animated;
	}

	return 0;

}

int incAnimFrames(int x, int y) {

	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		gEditor->tile[index]->animFrames++;
		fbl_update_text(gEditor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", gEditor->tile[index]->animFrames);
	}
	else {
		gEditor->tileSettings.animFrames++;
		fbl_update_text(gEditor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", gEditor->tileSettings.animFrames);
	}

	return 0;

}

int decAnimFrames(int x, int y) {

	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		if (gEditor->tile[index]->animFrames > 1) {
			gEditor->tile[index]->animFrames--;
			fbl_update_text(gEditor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", gEditor->tile[index]->animFrames);
		}
	}
	else {
		if (gEditor->tileSettings.animFrames > 1) {
			gEditor->tileSettings.animFrames--;
			fbl_update_text(gEditor->animFramesTextId, 255, 255, 255, 255, (char*)"Anim frames: %d (-+)", gEditor->tileSettings.animFrames);
		}
	}

	return 0;

}

int incAnimSpeed(int x, int y) {

	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		gEditor->tile[index]->animSpeed++;
		fbl_update_text(gEditor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", gEditor->tile[index]->animSpeed);
	}
	else {
		gEditor->tileSettings.animSpeed++;
		fbl_update_text(gEditor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", gEditor->tileSettings.animSpeed);
	}

	return 0;

}

int decAnimSpeed(int x, int y) {

	int index = getIndexAtCursor();

	if (gEditor->tile[index] != nullptr) {
		if (gEditor->tile[index]->animSpeed > 1) {
			gEditor->tile[index]->animSpeed--;
			fbl_update_text(gEditor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", gEditor->tile[index]->animSpeed);
		}
	}
	else {
		if (gEditor->tileSettings.animSpeed > 1) {
			gEditor->tileSettings.animSpeed--;
			fbl_update_text(gEditor->animSpeedTextId, 255, 255, 255, 255, (char*)"Anim speed: %d (-+)", gEditor->tileSettings.animSpeed);
		}
	}

	return 0;

}

int saveMap(int x, int y) {

	bool success = Disk::getInstance().saveMap(*gEditor, "assets/map.scn");

	if (success)
		std::cout << "Saved map to assets/map.scn" << std::endl;
	else
		std::cout << "Error saving map!" << std::endl;

	return 0;

}

int loadMap(int x, int y) {

	bool success = Disk::getInstance().loadMap(*gEditor, "assets/map.scn");

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

	return 0;

}

int exportBin(int x, int y) {

	bool success = Disk::getInstance().exportMapBin(*gEditor, "assets/map.scb");

	if (success) {
		std::cout << "Exported map to assets/map.scb" << std::endl;
	}
	else
		std::cout << "Error exporting map!" << std::endl;

	return 0;

}
