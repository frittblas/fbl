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
        float    timeLeft;
    };

    struct AirMeter {
        uint16_t x, y;
        uint16_t meterId, sweetSpotId, sweetSpotSize;
        uint16_t pointerId, pointerY;
        uint16_t arrowUpId, arrowDownId;
        int16_t  checkDuration;
        float    sweetSpotY;
        float    speed;
    };

    struct ColorCables {
        uint16_t x, y;
        uint16_t lineId[3];
        uint16_t mimicLineId[3];
        uint16_t color[3];
        uint16_t mimicColor[3];
        uint16_t button[3];
        int16_t  checkDuration;
    };

    struct CalcChecksum {
        uint16_t x, y;
        int16_t  showDuration;
    };

    struct Sequencer {
        uint16_t x, y;
        uint16_t seq[3];
        int16_t  showDuration;
    };

    void setupMaintenance(Game& g);
    void setupAirPressure(int x, int y);
    void setupColorCables(int x, int y);
    void setupCalcChecksum(int x, int y);
    void setupSequencer(int x, int y);
    void getInput(Game& g);
    void processAirPressure(Game& g);
    void processColorCables(Game& g);
    void updateCableColors(int index, bool mimic);
    void processTimers(Game& g);
    void advance();
    void fail();
    void checkWinCondition();
    void hideSprites();

    virtual void tick(Game& g) override;

private:

    PostRace* mPostRace;
    int mPostRaceDelay;

    const float cTimeStep = 0.25;
    const int   cKeyDelay = 10;

    int mFails;
    int mFailCrossId[3];

    int mKeyDelayLeft[3] = {};

    int mOpsLeft, mOpsId;

    TimerBar mTimerBar[4];

    AirMeter        mAirMeter;
    ColorCables     mColorCables;
    CalcChecksum    mCalc;
    Sequencer       mSeq;

};
