/*
*
*	Ecs
*
*	SpriteSystem.cpp
*
*	The sprite system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../../Efx.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "CardImageSystem.hpp"

extern float mCardTweenXId, mCardTweenYId;

void CardImageSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& cimg = ecs.GetComponent<CardImage>(entity);

		cimg.id = fbl_create_ui_elem(FBL_UI_DRAGGABLE, cimg.textureX, cimg.textureY, 64, 90, NULL);	// create card-sprite

		fbl_set_ui_elem_xy(cimg.id, pos.x, pos.y);	// set the coordinates
			
		// set layer
		//fbl_set_ui_elem_layer(cimg.id, cimg.layer);


	}

	std::cout << "Card image component system initialized!" << std::endl;

}

void CardImageSystem::Update(Coordinator& ecs) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& cimg = ecs.GetComponent<CardImage>(entity);

		fbl_set_ui_elem_xy(cimg.id, pos.x + 4, pos.y + 4);	// cards are slightly offset and has tween

	}

}
