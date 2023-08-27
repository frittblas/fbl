/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Observer.hpp
*
*	Observer class header, observer interface that defines the methods to be called
*   when an event occurs. This interface will be implemented by classes that want to receive notifications.
*
*	Hans Strömquist 2023
*
*/

#include "Observer.hpp"

// Observer-class implementation

Observer::Observer() {

	//std::cout << "Initialized Observer interface." << std::endl;
}

Observer::~Observer() {

	//std::cout << "Destroyed Observer interface." << std::endl;

}
