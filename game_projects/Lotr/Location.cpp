/*
*
*	Lotr
*
*	Location.cpp
*
*	Location class implemetation, takes care of all locations (levels), loading, unloading, transitions etc.
*
*	Hans Strömquist 2022
*
*/

#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "../../tools/ScenEdit/Disk.hpp"

#include "Location.hpp"

// Location-class implementation

Location::Location() {

	std::cout << "Initialized Location subsystem." << std::endl;

}

Location::~Location() {

	std::cout << "Destroyed Location subsystem." << std::endl;

}

void Location::change(ScenEdit* map, uint32_t posX, uint32_t posY) {

	std::cout << "lol works! from location." << std::endl;

}

void Location::transition(uint8_t type) {


}

void Location::loadLocation(ScenEdit* map) {

	bool success = Disk::getInstance().loadMap_fbl(*map, "map/01.scn", 0); // note that this calls fbl_destroy_all_sprites()

	if (success)
		std::cout << "Loaded map!" << std::endl;
	else
		std::cerr << "Error loading map!" << std::endl;

	// set up the map for path finding
	for (uint32_t i = 0; i < map->mapWidth; i++) {
		for (uint32_t j = 0; j < map->mapHeight; j++) {
			fbl_pathf_set_walkability(i, j, FBL_PATHF_WALKABLE);
		}
	}

	// set tiles to walkable/unwalkable
	for (uint32_t i = 0; i < map->mapWidth; i++) {
		for (uint32_t j = 0; j < map->mapHeight; j++) {

			int index = i + map->mapWidth * j;

			if (map->tile[index] != nullptr)
				if (map->tile[index]->type > 0)	// atm everything over 0 is unwalkable
					fbl_pathf_set_walkability(i, j, FBL_PATHF_UNWALKABLE);
		}
	}

}

void Location::unLoadLocation(ScenEdit* map) {

	map->resetMap(0, 0);			// note that this calls fbl_destroy_all_sprites()
	fbl_destroy_all_emitters();		// deleta all emitters (rain, snow, fire etc.)

}
