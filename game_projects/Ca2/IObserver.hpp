/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	IObserver.hpp
*
*	Observer class header, observer interface that defines the methods to be called
*   when an event occurs. This interface will be implemented by classes that want to receive notifications.
*
*	Hans Strömquist 2023
*
*/

#pragma once


class IObserver {

public:

    //virtual void onRobotDeath() = 0;    // has to be implemented

    virtual void onRobotDeath() {

    }

    virtual void onFireLaser() {
        // default implementation does nothing.
    }

};
