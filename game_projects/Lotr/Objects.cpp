/*
*
*	Lotr
*
*	Objects.cpp
*
*	Objects class implementation, keeps references to the game objects, movable rocks, shops etc.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "Objects.hpp"

// Objects-class implementation

Objects::Objects() {

	std::cout << "Initialized Objects subsystem." << std::endl;
}

Objects::~Objects() {

	std::cout << "Destroyed Objects subsystem." << std::endl;

}
