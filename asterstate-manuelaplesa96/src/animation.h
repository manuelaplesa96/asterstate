#pragma once

#include "entities.h"

class Animation : public Entity{
public:
    //slicica, broj redaka i stupaca u slicici i visina i sirina, brzina vrtnje slicica
    Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel,
                float position_x,float position_y,bool isAlive);
    virtual void update(sf::Time dt = sf::Time::Zero)  override;
    bool checkIsAlive(); 
private:
    int mRows, mCols, mWidth, mHeight;
    float mSpeed;
    float mCurrentCol;
    int   mCurrentRow;
};