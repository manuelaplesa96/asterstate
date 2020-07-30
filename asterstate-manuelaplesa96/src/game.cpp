#include "game.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <string>
#include <typeinfo>

// Kod dopuniti/promijeniti po potrebi.  

//nije gotov
Game::Game() : mWindow(sf::VideoMode(1280, 1024), "SFML window") 
{
    mDtFixed = sf::seconds(1.0f/60.0f);
    mResult=0;
    mLives=3;
    
    //alokacija svih stanja fali
    Game *pom;
    std::shared_ptr<WelcomeState> welcome;
    welcome.reset(new WelcomeState(this));
    mAllStates.push_back(std::move(welcome));
  
    std::shared_ptr<PlayState> play;
    play.reset(new PlayState(this));
    mAllStates.push_back(std::move(play));

    std::shared_ptr<ScoreState> score;
    score.reset(new ScoreState(this));
    mAllStates.push_back(std::move(score)); 

    auto it=mAllStates.begin();
    mpGameState=*it;
    mpGameState->init();
}

sf::RenderWindow * Game::getWindow(){
    return &mWindow;
}

void Game::run()
{
    // Start the game loop
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate >= mDtFixed){
            timeSinceLastUpdate -= mDtFixed;
            processEvents();
            update(mDtFixed);
        }       
        render();
    }
}

void Game::processEvents()
{
    //std::cout << (*mpGameState).mState <<std::endl;

    // Process events
    sf::Event event;
    while (mWindow.pollEvent(event)){
        mpGameState->handleEvents(event);
    }
}

void Game::update(sf::Time dt)
{
    mpGameState->update(dt);
}

void Game::render()
{   
    mpGameState->render();
}

void Game::setState(GameState::State newState)
{
    //mpGameState treba dobit vrijednost aktualnog stanja
    mpGameState.reset();
    auto it=mAllStates.begin();
    while(it!=mAllStates.end())
    {   
        if((**it).mState==newState)
        {   
            mpGameState=*it;
            mpGameState->init();
            break;
        }
        it++;
    }
}

 