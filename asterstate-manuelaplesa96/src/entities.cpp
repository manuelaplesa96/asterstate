 #include "entities.h"
 #include <iostream>
 #include <cmath>
#include <typeinfo>


// Dopuniti po potrebi.

Entity::Entity(sf::Texture const & texture){
    mSprite.setTexture(texture);
    auto lb = mSprite.getLocalBounds();
    //ishodišna točka u centar
    mSprite.setOrigin(lb.width/2, lb.height/2);
    mVelocity.x=mVelocity.y = 30.0f; 
    isAlive = true;

    mRadius = 1.1*std::min(lb.width/2,lb.height/2);
    mCircle.setRadius(mRadius);
    mCircle.setFillColor(sf::Color(0, 255, 0, 0));
    mCircle.setOutlineThickness(1);
    mCircle.setOutlineColor(sf::Color::Yellow);
    mCircle.setOrigin(mRadius, mRadius);
}

//renderTarget-ekran na kojem se vrši iscrtavanje
//renderStates-stanje koje u sebi sadrži transformacije objekta
void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    //sadrzi citavu transformaciju objekta 
    states.transform *= getTransform();
    target.draw(mSprite, states);
    //target.draw(mCircle, states);
}

//ovakav kod ne pomiče nego gradi lokalnu transformaciju
//koja se onda sa draw iscrta nakon sto je dobivena sa getTransform
void Entity::update(sf::Time dt){
    auto pos = getPosition();
    float x = pos.x + mVelocity.x*dt.asSeconds();
    float y = pos.y + mVelocity.y*dt.asSeconds();
    setPosition(x,y);
}

float Entity::getRadius(){
    return mRadius;
}


bool Entity::bulletCollision(Entity &asteroid,Entity &bullet)
{
    //udaljenost njihovih centara koja se uspoređuje sa sumom pripadnih radijusa. 
    //Ako je udaljenost manja od sume radijusa uzimamo da imamo koliziju i na mjestu
    float l_x=asteroid.getPosition().x-bullet.getPosition().x;
    float l_y=asteroid.getPosition().y-bullet.getPosition().y;
    float l=sqrt((l_x*l_x)+(l_y*l_y));
    float r=asteroid.getRadius()+bullet.getRadius();

    if(l<r)
        return true;

    return false;
}


Asteroid::Asteroid(sf::Texture const & texture,float velocity_x,float velocity_y,float angle, float position_x,float position_y):
Entity(texture),mAngle(angle){
    mVelocity.x=velocity_x;
    mVelocity.y=velocity_y;
    setOrigin(getPosition().x/2,getPosition().y/2);
    setPosition(position_x,position_y);
}

void Asteroid::update(sf::Time dt)
{
    auto pos = getPosition();
    
    float x = (pos.x + mVelocity.x*dt.asSeconds());
    float y = (pos.y + mVelocity.y*dt.asSeconds());
    
    if(x>=1280) x=0.0f; 
    else if(x<=0) x=1280.0f;
    else x=x;
    
    if(y<=0) y=1024.0f;
    else if(y>=1024) y=0.0f;
    else y=y;
    
    //std::cout << x << " " << y <<std::endl;
    rotate(mAngle);
    setPosition(x,y);
}


Player::Player(sf::Texture const & texture, float position_x,float position_y):Entity(texture){
    setOrigin(getPosition().x/2,getPosition().y/2);
    setPosition(position_x/2,position_y/2);
    mVelocity=80.0f;
}

void Player::update(sf::Time dt)
{
    if(!isAlive)
        return;

    sf::Vector2f velocity(0.f,0.f);
    auto pos = getPosition();
    auto angle=getRotation()*(3.14/180);
    if(pos.y<=0.0f)
        setPosition(pos.x,1024.0f);
    if(pos.y>=1024.0f)
        setPosition(pos.x,0.0f);
    if(pos.x<=0.0f)
        setPosition(1280.0f,pos.y);
    if(pos.x>=1280.0f)
        setPosition(0.0f,pos.y);

    if(mIsMovingUp){
        if(mVelocity<300.0f)
            mVelocity+=3.0f;
        velocity.y-=cos(angle)*mVelocity;
        velocity.x+=sin(angle)*mVelocity;    
    }        
    if(mIsMovingDown){
        float tmp=mVelocity;
        if(mVelocity>=(tmp/10))
        mVelocity-=(tmp/10);   
        velocity.y+=cos(angle)*mVelocity;
        velocity.x-=sin(angle)*mVelocity;
    }    
    if(mIsMovingLeft){
        setRotation(getRotation()-1.7);   
    }
    if(mIsMovingRight){
        setRotation(getRotation()+1.7);
    }
    
    move(velocity*dt.asSeconds());
}

float Player::getRadius()
{
    return mRadius;
}

void Player::handlePlayerInput(sf::Keyboard::Key code, bool isPressed){
    if(code == sf::Keyboard::Key::Up)
        mIsMovingUp = isPressed;
    if(code == sf::Keyboard::Key::Down)
        mIsMovingDown = isPressed;
    if(code == sf::Keyboard::Key::Left)
        mIsMovingLeft = isPressed;
    if(code == sf::Keyboard::Key::Right)
        mIsMovingRight = isPressed;            
}

bool Player::playerCollision(Entity &asteroid)
{
    //udaljenost njihovih centara koja se uspoređuje sa sumom pripadnih radijusa. 
    //Ako je udaljenost manja od sume radijusa uzimamo da imamo koliziju i na mjestu
    float l_x=abs(asteroid.getPosition().x-getPosition().x);
    float l_y=abs(asteroid.getPosition().y-getPosition().y);
    float l=sqrt((l_x*l_x)+(l_y*l_y));
    float r=asteroid.getRadius()+getRadius();
    if(l<r)
        return true;
    
    return false;
}


Bullet::Bullet(sf::Texture const & texture, sf::Vector2f position, float rotation):Entity(texture)
{
    setPosition(position);
    setRotation(rotation);
    mSpeed=180.0f;
    mVelocity.x=mVelocity.y=0.f;   
}

void Bullet::update(sf::Time dt) 
{
    if(isAlive)
    {
        //auto pos = getPosition();
        auto angle=(getRotation()+90)*(3.14/180);
        mVelocity.y-=cos(angle)*mSpeed;
        mVelocity.x+=sin(angle)*mSpeed;

        move(mVelocity*dt.asSeconds());
    }
    else{
        move(123456,123456);
    }
    
}

