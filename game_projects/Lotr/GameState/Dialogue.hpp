/*
*
*	Lotr
*
*	Dialogue.hpp
*
*	Dialogue class header, takes care of the Dialogue game state.
*
*	Hans Strömquist 2022
*
*/

#include "IState.hpp"

class Dialogue : public IState{

public:
    Dialogue();
    ~Dialogue();

    virtual void tick();

};
