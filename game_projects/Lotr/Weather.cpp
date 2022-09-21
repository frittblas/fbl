/*
*
*	Lotr
*
*	Weather.cpp
*
*	Weather class implementation, handles the weather system.
*
*	Hans Strömquist 2022
*
*/

#include "Weather.hpp"
#include <iostream>

// Weather-class implementation

Weather::Weather() {

	std::cout << "Initialized Weather subsystem." << std::endl;
}

Weather::~Weather() {

	std::cout << "Destroyed Weather subsystem." << std::endl;

}
