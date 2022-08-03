/*
*
*	Lotr
*
*	Explore.cpp
*
*   Explore class implementation, takes care of the Exploring game-state.
*   Singleton.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "Explore.hpp"

// Explore-class implementation

Explore::Explore() {

	std::cout << "Alloc Explore!" << std::endl;

}

Explore::~Explore() {

	std::cout << "Destroy Explore!" << std::endl;

}

void Explore::tick() {


}
