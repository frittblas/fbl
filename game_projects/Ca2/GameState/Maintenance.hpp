/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Maintenance.hpp
*
*	Maintenance class header, takes care of the Maintenance game state.
*
*	Hans Strömquist 2023
*
*/

#pragma once

#include "IState.hpp"

#include "Race/PostRace.hpp"

class Game;
struct Addon;

class Maintenance : public IState{

public:
    Maintenance();
    ~Maintenance();

    enum RaceState {
        Undecided,
        First,
        Second,
        Third,
        Fourth,
        Dead,
        GameOver
    };

    struct TimerBar {
        uint16_t x, y;
        uint16_t primId;
        uint16_t red;
        uint16_t totalTime;
        uint16_t timeLeft;
    };

    struct AirMeter {
        uint16_t x, y;
        uint16_t meterId, sweetSpotId, sweetSpotSize;;
        float    sweetSpotY;
        uint16_t pointerId, pointerY;
        uint16_t arrowUpId, arrowDownId;
        float    speed;
    };

    void setupMaintenance(Game& g);
    void getInput(Game& g);
    void processAirPressure(Game& g);
    void processTimers(Game& g);


    virtual void tick(Game& g) override;

    //static int sRaceState;

private:

    PostRace* mPostRace;
    int mPostRaceDelay;

    TimerBar mTimerBar[4];

    AirMeter mAirMeter;

};
