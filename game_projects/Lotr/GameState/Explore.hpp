/*
*
*	Lotr
*
*	Explore.hpp
*
*	Explore class header, takes care of the Exploring game state.
*   Singleton.
*
*	Hans Strömquist 2022
*
*/

#include "IState.hpp"

class Explore : public IState{

private:

public:
    Explore();
    ~Explore();

    virtual void tick();

};
