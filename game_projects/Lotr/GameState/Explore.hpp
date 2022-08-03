/*
*
*	Lotr
*
*	Explore.hpp
*
*	Explore class header, takes care of the Exploring game state.
*
*	Hans Strömquist 2022
*
*/

#include "IState.hpp"

class Explore : public IState{

public:
    Explore();
    ~Explore();

    virtual void tick();

};
