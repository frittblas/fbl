/*
*
*	Ecs
*
*	LightSystem.cpp
*
*	The light system. It's a special sprite that acts as a light using the fbl lightmap.
*	It follows the entity.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "../../Game.hpp"
#include "../../GameState/GameState.hpp"

#include "LightSystem.hpp"

void LightSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& light = ecs.GetComponent<Light>(entity);

		light.id = fbl_create_sprite(light.textureX, light.textureY, light.w, light.h, 0);	// create a sprite, with the light image
		fbl_set_sprite_is_light(light.id, true);			// it is a light :)
		fbl_set_sprite_xy(light.id, pos.x - (light.w / 2) * light.scale + 16, pos.y - (light.h / 2) * light.scale + 16);	// set the coordinates
		fbl_set_sprite_scale(light.id, light.scale);		// set scale (size of the light)
			
	}

	std::cout << "Light component system initialized!" << std::endl;
	
}

void LightSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& light = g.mEcs->GetComponent<Light>(entity);

		// update light x, y to the xy of the Position component, different pos depending on gameState

		if(g.mState->get() == GameState::StateType::Explore)
			fbl_set_sprite_xy(light.id, pos.x - (light.w / 2) * light.scale + 16, pos.y - (light.h / 2) * light.scale + 16);	// set the coordinates
		else if (g.mState->get() == GameState::StateType::Race)
			fbl_set_sprite_xy(light.id, pos.x + 16, pos.y + 16);

	}

}
