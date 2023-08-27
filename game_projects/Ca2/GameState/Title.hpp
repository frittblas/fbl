/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Title.hpp
*
*	Title class header, takes care of the title screen.
*
*	Hans Strömquist 2022
*
*/

#include "IState.hpp"

class Game;

class Title : public IState {

public:
    Title();
    ~Title();

    virtual void tick(Game& g) override;

private:
    int mTitleId, mSubTitleId, mAuthorTextId, mWebsiteTextId;
    int mTweenIdTitle, mTweenIdSub;

};
