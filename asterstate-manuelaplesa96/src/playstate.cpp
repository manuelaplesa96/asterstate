#include "game.h"
#include "playstate.h"
#include <iostream>

// Vaš kod dolazi ovdje.
PlayState::PlayState(Game * pGame) : GameState(pGame)
{
    mWindow=mpGame->getWindow();
    mState=Playing;
    mBackgroundSprite.setTexture(mTextureHolder.getTexture(Textures::background));
    //init(); 
    /*
    mDtFixed = sf::seconds(1.0f/60.0f);
    mLives=3;
    mResult=0;
    mPlayer.reset(new Player(mTextureHolder.getTexture(Textures::player),mWindow.getSize().x,mWindow.getSize().y));   
    fillObjects();
    mExplosion.reset(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.2f,0,0,false));*/
    
    //setText();
}

void PlayState::init()
{
    mLives=3;
    mResult=0;
    mFinish=false;
    mDead=false;
    mPlayer.reset(new Player(mTextureHolder.getTexture(Textures::player),mWindow->getSize().x,mWindow->getSize().y));            
    mObjects.clear();
    fillObjects();

    mExplosion.reset(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.2f,0,0,false));
    
    setText();
}

void PlayState::handleEvents(sf::Event event)
{
    switch(event.type){
        case sf::Event::Closed: 
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
    }
}

//treba prijelaz na ScoreState napraviti
void PlayState::handlePlayerInput(sf::Keyboard::Key code, bool isPressed){
    if(code == sf::Keyboard::Key::Up)
        mPlayer->handlePlayerInput(code,isPressed);
    if(code == sf::Keyboard::Key::Down)
        mPlayer->handlePlayerInput(code,isPressed);
    if(code == sf::Keyboard::Key::Left)
        mPlayer->handlePlayerInput(code,isPressed);
    if(code == sf::Keyboard::Key::Right)        
        mPlayer->handlePlayerInput(code,isPressed);
    if(code==sf::Keyboard::Key::Space){
        if(isPressed==true && mLives>0 && mResult<300)
        { //space==pucanje
            //stvaramo metak s kojim ćemo pucati
            std::unique_ptr<Entity> bullet;
            bullet.reset(new Bullet(mTextureHolder.getTexture(Textures::bullet),
                                    mPlayer->getPosition(),(mPlayer->getRotation())-90));
            mObjects.push_back(std::move(bullet)); 
        }
    }
}

void PlayState::update(sf::Time dt)
{
    checkCollision();
    objectsErase();
    //kako ne bi sav kod koji se tiče updateiranja pozicije odredenog objekta
    //morali ovdje pisati zatvorimo ga u klasu
    if(!mDead)
    {   
        mPlayer->update(dt);

        auto it=mObjects.begin();
        while(it!=mObjects.end())
        {
            (*it)->update(dt);
            it++;
        }

        //mogu prazne zagrade jer ima defaultni konstruktor
        //mExplosion->update();
    }
    mExplosion->update();

    if(mFinish || mDead)
    {
        
        mpGame->setState(Score);
        //mFinish=false;
    }
}

void PlayState::render()
{
    // Clear screen
    mWindow->clear();
    mWindow->draw(mBackgroundSprite);

    mWindow->draw(mRes);
    mWindow->draw(mLive);
     
    auto it=mObjects.begin();
    while(it!=mObjects.end()){
        mWindow->draw(**it);
        it++;
    }

    mWindow->draw(*mPlayer);
    if((*mExplosion).checkIsAlive())
        mWindow->draw(*mExplosion);   
        
    // Update the window
    mWindow->display();
}

//dodatne fje 
void PlayState::checkCollision()
{
    //objectsErase();
    auto it=mObjects.begin();
    int i=0;
    auto obrisati=mObjects.begin();
    //mExplode=false;
    while(it!=mObjects.end())
    {  
        if(typeid(**it)==typeid(Asteroid))
        {
            if(mPlayer->playerCollision(**it)==true && (*it)->isAlive)
            {
                float rad=mExplosion->getRadius();
                mExplosion.reset(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.2f,
                                            mPlayer->getPosition().x+(rad/2),mPlayer->getPosition().y+(rad/2),true));
                if(mLives>1){
                    mLives-=1;    
                    std::string lives="Lives: " + std::to_string(mLives);
                    mLive.setString(lives);
                    //avion vraćamo u početnu poziciju
                    mPlayer.reset(new Player(mTextureHolder.getTexture(Textures::player),mWindow->getSize().x,mWindow->getSize().y));   

                    (*it)->setPosition(123456,123456);      
                }                
                else{
                    mLives=0;
                    mDead=true; //nema vise zivota==kraj igre
                }
            }
            else
            {
                auto jt=mObjects.begin();
                while(jt!=mObjects.end())
                {
                    if(typeid(**jt)==typeid(Bullet) && (*it)->bulletCollision(**it,**jt)==true && (*it)->isAlive)
                    {
                        float rad=mExplosion->getRadius();
                        mExplosion.reset(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.2f,
                                            (*it)->getPosition().x+(rad/2),(*it)->getPosition().y+(rad/2),true));

                        //maknemo metak
                        (*jt)->isAlive=false;
                        (*it)->isAlive=false;

                        float r=(*it)->getRadius();
                        if(r>40){ //ako je veliki asteroid=10 i prelazi u 2 mala
                            mResult+=10;
                            float velocity_x=(-100.0f+rand()%201);
                            float velocity_y=(-100.0f+rand()%201);

                            //nova dva asteroida koji će se pojaviti
                            std::unique_ptr<Asteroid> asteroid1;
                            asteroid1.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall2),
                                            velocity_x,velocity_y,3,(*it)->getPosition().x-5,(*it)->getPosition().y));
                            mObjects.push_back(std::move(asteroid1)); 
                            std::unique_ptr<Asteroid> asteroid2;
                            asteroid2.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall2),
                                            -velocity_x,-velocity_y,3,(*it)->getPosition().x,(*it)->getPosition().y));
                            mObjects.push_back(std::move(asteroid2)); 
                        }
                        else if(r<=40 && r>20){ //ako je srednji asteroid=20 i nestaje
                            mResult+=20;
                        }
                        else if(r<=20){ //ako je mali asteroid=30 i nestaje
                            mResult+=40;
                        }
                        
                        std::string result="Score: " + std::to_string(mResult);
                        mRes.setString(result);
                        break;          
                    }
                    jt++;
                }               
            }    
        }
        it++;
        i++;
    }
    //objectsErase();
    mpGame->mResult=mResult;
    mpGame->mLives=mLives;
}

void PlayState::fillObjects()
{
    srand((int)(time(NULL)));
    int max_l=mWindow->getSize().x+1;
    int max_h=mWindow->getSize().y+1;

    std::unique_ptr<Asteroid> asteroid1;
    asteroid1.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorBig1),
                    (-200.0f+rand()%401),(-200.0f+rand()%401),3,rand()%max_l,rand()%max_h));
    mObjects.push_back(std::move(asteroid1));  
    
    std::unique_ptr<Asteroid> asteroid2;
    asteroid2.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorBig2),
                    (-200.0f+rand()%401),(-200.0f+rand()%401),3,rand()%max_l,rand()%max_h));
    mObjects.push_back(std::move(asteroid2));  
   
    std::unique_ptr<Asteroid> asteroid3;
    asteroid3.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorMedium1),
                    (-200.0f+rand()%401),(-200.0f+rand()%401),3,rand()%max_l,rand()%max_h));
    mObjects.push_back(std::move(asteroid3));  
    
    std::unique_ptr<Asteroid> asteroid4;
    asteroid4.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorMedium2),
                    (-200.0f+rand()%401),(-200.0f+rand()%401),3,rand()%max_l,rand()%max_h));
    mObjects.push_back(std::move(asteroid4));  

    std::unique_ptr<Asteroid> asteroid5;
    asteroid5.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall1),
                    (-200.0f+rand()%401),(-200.0f+rand()%401),3,rand()%max_l,rand()%max_h));
    mObjects.push_back(std::move(asteroid5));  

    std::unique_ptr<Asteroid> asteroid6;
    asteroid6.reset(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall2),
                    (-200.0f+rand()%401),(-200.0f+rand()%401),3,rand()%max_l,rand()%max_h));
    mObjects.push_back(std::move(asteroid6));  

}

void PlayState::setText()
{
    std::string lives="Lives: " + std::to_string(mLives);
    mLive.setFont(mFont);
    mLive.setString(lives);
    mLive.setCharacterSize(24);
    mLive.setPosition(10,mLive.getPosition().y+10);
    std::string result="Score: " + std::to_string(mResult);
    mRes.setFont(mFont);
    mRes.setString(result);
    mRes.setCharacterSize(24);
    mRes.setPosition(mWindow->getSize().x-200,mRes.getPosition().y+10);
}

void PlayState::objectsErase()
{
    auto it=mObjects.begin();
    while(it!=mObjects.end()){
        if(((*it)->isAlive)==false)
            mObjects.erase(it++);
        else it++;
    }
    //if(mObjects.size()==0) //svi objekti su pogodeni==kraj igre
    //    mFinish=true;
    //std::cout << mObjects.size() <<std::endl;
    if(mResult==300)
        mFinish=true;
}