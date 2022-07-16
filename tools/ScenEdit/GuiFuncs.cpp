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

extern ScenEdit *editor;

// return the index for the tile vector based on the cursor coordinates.
int getIndexAtCursor() {

	return (editor->mapMarkerX / editor->tileSize) + editor->mapWidth * (editor->mapMarkerY / editor->tileSize);

}

//
// Below are the functions that are called by the Gui buttons
//

int selectSpriteLeft(int x, int y) {

	if (editor->drawTileX > 0)
		editor->drawTileX -= editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

	return 0;

}

int selectSpriteRight(int x, int y) {

	if (editor->drawTileX < (fbl_get_texture_w() - editor->tileSize))
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

	if (editor->drawTileY < (fbl_get_texture_h() - editor->tileSize))
		editor->drawTileY += editor->tileSize;

	fbl_set_sprite_image(editor->drawTileId, editor->drawTileX, editor->drawTileY, editor->tileSize, editor->tileSize, 0);

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

	return 0;

}

int toggleKinematic(int x, int y) {

	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		if (!editor->tile[index]->kinematic)
			editor->tile[index]->kinematic = true;
		else if (editor->tile[index]->kinematic)
			editor->tile[index]->kinematic = false;

		std::cout << "Tile is kinematic: " << editor->tile[index]->kinematic << std::endl;

	}

	return 0;

}

int toggleAnimation(int x, int y) {


	int index = getIndexAtCursor();

	if (editor->tile[index] != nullptr) {
		if (!editor->tile[index]->animated) {
			editor->tile[index]->animated = true;
			fbl_set_sprite_animation(editor->tile[index]->id, true, editor->tile[index]->texture_x, editor->tile[index]->texture_y,
				editor->tileSize, editor->tileSize, editor->tile[index]->animFrames, editor->tile[index]->animSpeed, true);
		}
		else if (editor->tile[index]->animated) {
			editor->tile[index]->animated = false;
			fbl_set_sprite_animation(editor->tile[index]->id, false, editor->tile[index]->texture_x, editor->tile[index]->texture_y,
				editor->tileSize, editor->tileSize, editor->tile[index]->animFrames, editor->tile[index]->animSpeed, true);
		}

		std::cout << "Tile is animated: " << editor->tile[index]->animated << std::endl;

	}

	return 0;

}

int incAnimFrames(int x, int y) {


	return 0;

}

int decAnimFrames(int x, int y) {


	return 0;

}

int incAnimSpeed(int x, int y) {


	return 0;

}

int decAnimSpeed(int x, int y) {


	return 0;

}

int saveMap(int x, int y) {


	return 0;

}

int loadMap(int x, int y) {


	return 0;

}

int exportLua(int x, int y) {


	return 0;

}
