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

#pragma once


class Observer {

public:
    Observer();
    ~Observer();

    virtual void onRobotDeath() = 0;

};
