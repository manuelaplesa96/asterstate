#include "game.h"
#include "gamestate.h"
#include <iostream>

// Vaš kod dolazi ovdje

GameState::GameState(Game * pGame):mpGame(pGame){
    if(!mFont.loadFromFile("neuropol_x_rg.ttf")){
        throw std::runtime_error("Font ne postoji!");
    }
    //mWindow=mpGame->getWindow();
    //mLives=3;
    //mResult=0;
}

//welcome klasa
WelcomeState::WelcomeState(Game * pGame) : GameState(pGame)
{
    mWindow=mpGame->getWindow();
    mState=Welcome;
    //init();
}

void WelcomeState::init()
{ 
    setText();   
}

void WelcomeState::setText()
{
    std::string name="Asteroids clone by Plesa M.";
    mName.setFont(mFont);
    mName.setString(name);
    mName.setCharacterSize(27);
    mName.setPosition(mWindow->getSize().x/2-270,mWindow->getSize().y/2);

    std::string begin="Press any key to continue";
    mBegin.setFont(mFont);
    mBegin.setString(begin);
    mBegin.setCharacterSize(20);
    mBegin.setPosition(mWindow->getSize().x/2-200,mWindow->getSize().y/2+50);    
}

//bilo koja tipka==nova igra
void WelcomeState::handleEvents(sf::Event event)
{
    switch(event.type){
        case sf::Event::Closed: 
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            mContinue=true;    
            break;
        case sf::Event::KeyReleased:
            mContinue=false;    
            break;
    }    
}

//nije implementirano
void WelcomeState::update(sf::Time dt)
{
    if(mContinue)
    {
        //prijeđi na PlayState
        mpGame->setState(Playing);
    }
}

void WelcomeState::render()
{
    mWindow->clear();
    mWindow->draw(mName);
    mWindow->draw(mBegin);  
        
    // Update the window
    mWindow->display();
}


//score klasa
ScoreState::ScoreState(Game * pGame) : GameState(pGame)
{
    mWindow=mpGame->getWindow();
    mState=Score;
    //init();
}

void ScoreState::init()
{
    mResult=mpGame->mResult;
    mContinue=false;
    setText();
}

void ScoreState::setText()
{
    std::string score="Score = " + std::to_string(mResult);
    mScore.setFont(mFont);
    mScore.setString(score);
    mScore.setCharacterSize(23);
    mScore.setPosition(mWindow->getSize().x/2-270,mWindow->getSize().y/2);

    std::string option="Enter to continue. Esc to finish.";
    mOption.setFont(mFont);
    mOption.setString(option);
    mOption.setCharacterSize(23);
    mOption.setPosition(mWindow->getSize().x/2-270,mWindow->getSize().y/2+50); 

    std::string won="You won.";
    mWon.setFont(mFont);
    mWon.setString(won);
    mWon.setCharacterSize(23);
    mWon.setPosition(mWindow->getSize().x/2-270,mWindow->getSize().y/2-50);     
}

//esc==exit, return==nova igra
void ScoreState::handleEvents(sf::Event event)
{
    //init();
    switch(event.type){
        case sf::Event::Closed: 
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code,true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code,false);
            break;
    }
}

void ScoreState::handlePlayerInput(sf::Keyboard::Key code, bool isPressed){
    if(code == sf::Keyboard::Key::Escape)
        mWindow->close();
    if(code == sf::Keyboard::Key::Return)
        mContinue=isPressed;   
}


void ScoreState::update(sf::Time dt)
{
    if(mContinue)
    {
        //prelazi u PlayState
        mpGame->mDtFixed=sf::seconds(1.0f/60.0f);
        //mContinue=false;
        mpGame->setState(Playing);
    }
}

void ScoreState::render()
{
    mWindow->clear();
    if(mResult==300)
        mWindow->draw(mWon);
    mWindow->draw(mScore);
    mWindow->draw(mOption);  
        
    // Update the window
    mWindow->display();   
}