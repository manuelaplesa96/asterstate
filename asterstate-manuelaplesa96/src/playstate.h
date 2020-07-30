#pragma once
#include "gamestate.h"
#include "textureHolder.h"
#include "entities.h"
#include "animation.h"
//#include "game.h"
#include "list"


class Game;
//class GameState;

class PlayState : public GameState{
    // Vaš kod. 
public:
    PlayState(Game * pGame);
    void init(); //override;
    void handleEvents(sf::Event event); //override;
    void update(sf::Time dt = sf::Time::Zero); //override;
    void render(); //override;
    //dodatne fje 
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    void checkCollision();
    void fillObjects();
    void setText();
    void objectsErase();

private:
    sf::RenderWindow *mWindow;
    TextureHolder mTextureHolder;  
    sf::Sprite mBackgroundSprite;

    std::unique_ptr<Player> mPlayer;
    std::unique_ptr<Animation> mExplosion;
    std::list<std::unique_ptr<Entity>> mObjects;

    unsigned int mResult;
    unsigned int mLives;
    sf::Text mLive;
    sf::Text mRes;
    sf::Text mGameOver;
    sf::Text mEnd;

    bool mFinish;
    bool mDead=false;
};


