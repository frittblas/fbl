/*
*
*	Ecs
*
*	LaserSystem.hpp
*
*	The laser system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include <vector>
#include "../../System.hpp"
#include "../.../../../../Observer.hpp"

class Coordinator;
class Game;

class LaserSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

	void dealDamage(Game& g, Entity attacker, Entity target);
	void setDirection(Position& pos, Laser& las);

	void attachObserver(Observer* observer);
	void detachObserver(Observer* observer);
	void freeObserverList();
	void robotDied();

private:
	std::vector<Observer*> observers;
};
