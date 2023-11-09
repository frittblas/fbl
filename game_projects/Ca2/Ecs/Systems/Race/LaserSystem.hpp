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
#include "../.../../../../IObserver.hpp"

class Coordinator;
class Game;

class LaserSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

	void dealDamage(Game& g, Entity attacker, Entity target);
	void setDirection(Position& pos, Laser& las);

	void attachObserver(IObserver* observer);
	void detachObserver(IObserver* observer);
	void freeObserverList();
	void robotDied();
	void firedLaser();

private:
	std::vector<IObserver*> observers;
};
