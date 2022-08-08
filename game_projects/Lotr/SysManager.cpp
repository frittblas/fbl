/*
*
*	Lotr
*
*	SysManager.cpp
*
*	SysManager class implementation, keeps pointers to all systems in the Ecs.
*
*	Hans Strömquist 2022
*
*/

#include "SysManager.hpp"

// SysManager-class implementation

SysManager::SysManager() {

	std::cout << "Initialized SysManager subsystem." << std::endl;
}

SysManager::~SysManager() {

	std::cout << "Destroyed SysManager subsystem." << std::endl;

}
