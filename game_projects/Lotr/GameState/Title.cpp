/*
*
*	Lotr
*
*	Title.cpp
*
*   Title class implementation, takes care of the title screen.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "Title.hpp"

// Title-class implementation

Title::Title() {

	std::cout << "Alloc Title!" << std::endl;

}

Title::~Title() {

	std::cout << "Destroy Title!" << std::endl;

}


// implement the virtual tick() function
void Title::tick() {

	int num = std::rand() / ((RAND_MAX + 1u) / 20); // random numbers from 0-19

	if (num == 0)
		std::cout << "Tick title!" << std::endl;

}
