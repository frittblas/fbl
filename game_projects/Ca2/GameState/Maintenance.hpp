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
        int      operand1, operand2;
        int      operation;
        char     operationChar;
        int      correctAnswer, alt1, alt2;
        int      finalAlt[3];
        uint16_t calcTextId;
        uint16_t altTextId[3];
        uint16_t button[3];
        int16_t  checkDuration;
    };

    struct Sequencer {
        uint16_t x, y;
        uint16_t seqId[5];
        uint16_t mimicSeqId[5];
        int16_t  seq[5];
        int16_t  mimicSeq[5];
        uint16_t arrowLeftId, arrowRightId;
        int16_t  checkDuration;
    };

    struct miniGamePos {
        int x, y;
    };

    void setupMaintenance(Game& g);
    void setupAirPressure(int x, int y);
    void setupColorCables(int x, int y);
    void setupCalcChecksum(Game& g, int x, int y);
    void setupSequencer(int x, int y);
    void processAirPressure(Game& g);
    void processColorCables(Game& g);
    void processCalcChecksum(Game& g);
    void processSequencer(Game& g);
    void updateCableColors(int index, bool mimic);
    void genCalc(Game& g);
    int  randNum(int min, int max);
    void genAddSubAlt(int correct, int& alt1, int& alt2);
    void genMulDivAlt(int correct, int& alt1, int& alt2);
    void randomizeSequence();
    void upDifficulty();
    void processTimers();
    void advance();
    void fail(Game& g);
    void checkWinCondition();
    void hideSprites();
    void getInput(Game& g);

    virtual void tick(Game& g) override;

private:

    PostRace* mPostRace;
    int mPostRaceDelay;

    const float cTimeStep = 0.25;
    const int   cKeyDelay = 10;
    const int   cCheckTimeLimit = 60;

    int mFails;
    int mFailCrossId[3];

    // for intro
    int mBlackBgId, mMaintenanceTextId;
    int mStartTimer, mBlackBgFade;

    int mKeyDelayLeft[3] = {};

    int mTotalOps, mOpsLeft, mOpsId;

    // the shortcut keys
    int mShortCut[10];

    TimerBar mTimerBar[4];

    AirMeter        mAirMeter;
    ColorCables     mColorCables;
    CalcChecksum    mCalc;
    Sequencer       mSeq;

    miniGamePos     pos[4];

};
