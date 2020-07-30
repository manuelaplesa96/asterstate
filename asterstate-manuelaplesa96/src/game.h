#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <memory>
#include "textureHolder.h"
#include "entities.h"
#include "gamestate.h"
#include "playstate.h"


// Klasu dopuniti po potrebi.
class GameState;
class WelcomeState;
class PlayState;
class ScoreState;

class Game{
public:
    Game();
    void run();
    void setState(GameState::State newState);
    sf::RenderWindow * getWindow();

    friend WelcomeState;
    friend ScoreState;
    friend PlayState;

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    std::shared_ptr<GameState> mpGameState;
    //WelcomeState *mpWelcome;
    //PlayState *mpPlay;
    //ScoreState *mpScore;
    //GameState::State mCurrentState;
    std::vector<std::shared_ptr<GameState>> mAllStates;
    sf::RenderWindow mWindow;

    unsigned int mResult;
    unsigned int mLives;

    sf::Time mDtFixed;
};
