#pragma once
#include <SFML/Graphics.hpp>

//klasa koja se sama zna aktualizirati
//koristimo ju kao baznu klasu
class Entity : public sf::Drawable, public sf::Transformable {
public:
    Entity(sf::Texture const & texture);
    //postavlja brzinu
    void setVelocity(sf::Vector2f const &  vel) { mVelocity = vel;}
    virtual void update(sf::Time dt = sf::Time::Zero); 
    float getRadius();
    bool bulletCollision(Entity &asteroid, Entity &bullet);
    //nužan da se klasa može ponašati kao bazna klasa  
    virtual ~Entity() {}
    bool isAlive;
protected:
    sf::Sprite mSprite;
    //virtualna je od drawable pa se mora preraditi u izvedenoj klasi
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //brzina gibanja
    sf::Vector2f mVelocity;
    //kruznice koristimo za detekciju kolizije
    sf::CircleShape mCircle;
    float mRadius;
};

// dopuniti

class Asteroid : public Entity{
public:
    Asteroid(sf::Texture const & texture,float velocity_x,float velocity_y,float angle,float position_x, float position_y);
    virtual void update(sf::Time dt = sf::Time::Zero)  override; 
private:
    sf::Vector2f mVelocity;
    float mAngle;
};


// dopuniti
class Player : public Entity{
public:
    Player(sf::Texture const & texture, float position_x,float position_y);
    virtual void update(sf::Time dt = sf::Time::Zero)  override; 
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    bool playerCollision(Entity &asteroid);
    float getRadius();
private:
    float mVelocity;
    bool mIsMovingUp    = false;
    bool mIsMovingDown  = false;
    bool mIsMovingLeft  = false;
    bool mIsMovingRight = false;
};


class Bullet : public Entity{
public:
    Bullet(sf::Texture const & texture, sf::Vector2f position, float rotation);
    virtual void update(sf::Time dt = sf::Time::Zero)  override;
    //void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    //float getRadius();
private:
    float mSpeed;
    sf::Vector2f mVelocity;
    bool mShoot=false;
};