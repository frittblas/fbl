/*
*
*	Lotr
*
*	Title.hpp
*
*	Title class header, takes care of the title screen.
*
*	Hans Strömquist 2022
*
*/

#include "IState.hpp"

class Title : public IState {

public:
    Title();
    ~Title();

    virtual void tick();
};
