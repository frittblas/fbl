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

    void hideTitle();
    void showTitle();

    virtual void tick(Game& g) override;

private:
    int mIntroId, mSubIntroId;
    int mTitleId, mSubTitleId, mAuthorTextId, mWebsiteTextId;
    int mTweenIdTitle, mTweenIdSub;

};
