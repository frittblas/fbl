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

#include "IObserver.hpp"

// Observer-class implementation

IObserver::IObserver() {

	//std::cout << "Initialized Observer interface." << std::endl;
}

IObserver::~IObserver() {

	//std::cout << "Destroyed Observer interface." << std::endl;

}
