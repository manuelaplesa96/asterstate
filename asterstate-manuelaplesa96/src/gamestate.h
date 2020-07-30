#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
class Game;

class GameState
{
public:
    GameState(Game * pGame);
    virtual ~GameState() {}
    // Sva moguća stanja igre.
        enum State{
            Welcome,
            Playing,
            Score,
            SIZE
        };
    // Metode koje moraju osigurati sva stanja igre.
    virtual void init() = 0;
    virtual void handleEvents(sf::Event event) = 0;
    virtual void update(sf::Time dt = sf::Time::Zero) = 0;
    virtual void render() = 0;

    friend Game;
protected:
    Game * const mpGame; // Pokazivač na Game klasu
    // eventualno i druge varijable  
    sf::RenderWindow *mWindow;
    sf::Font mFont;
    State mState;  
    //unsigned int mLives;
    //unsigned int mResult;     
};


class WelcomeState : public GameState{
//  Vaš kod ...
public:
    WelcomeState(Game * pGame);
    void init() override;
    void handleEvents(sf::Event event) override;
    void update(sf::Time dt = sf::Time::Zero) override;
    void render() override;

    
private:
    void setText();
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);

    sf::RenderWindow *mWindow;
    sf::Text mName;
    sf::Text mBegin;

    //bool mExit=false;
    bool mContinue=false;    
};

class ScoreState : public GameState{
// Vaš kod ...
public:
    ScoreState(Game * pGame);
    void init() override;
    void handleEvents(sf::Event event) override;
    void update(sf::Time dt = sf::Time::Zero) override;
    void render() override;

private:
    void setText();
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    sf::RenderWindow *mWindow;
    sf::Text mScore;
    sf::Text mOption;
    sf::Text mWon;
    unsigned int mResult;
    bool mContinue=false;

};


