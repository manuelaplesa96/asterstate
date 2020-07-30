#include "animation.h"

//mWidth(width/cols), mHeight(height/rows)...visina i sirina jedne slicice
Animation::Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel,
                        float position_x,float position_y,bool alive) :
  Entity(texture), mRows(rows), mCols(cols), mWidth(width/cols), mHeight(height/rows), mSpeed(vel),
  mCurrentCol(0.0f),  mCurrentRow(0) {
      setPosition(position_x,position_y);
      isAlive=alive;
  }

 void Animation::update(sf::Time dt){
    if(!isAlive) return;

    //koliko čekati da se prijeđe na novu slicicu
    mCurrentCol += mSpeed;
    if(static_cast<int>(mCurrentCol) == mCols){
        mCurrentCol = 0.0f;
        ++mCurrentRow;
    }
    if(mCurrentRow == mRows){
        mCurrentCol = 0.0f;
        mCurrentRow = 0;
        isAlive = false;
        return;
    }
    
    int i = mCurrentCol;
    int j = mCurrentRow; 
    //postavlja se onaj dio sprite sheeta koji zelimo prikazati
    //uzima cjelobrojne varijable....IntRect
    mSprite.setTextureRect(sf::IntRect(i*mWidth, j*mHeight, mWidth, mHeight));
} 

bool Animation::checkIsAlive(){
    return isAlive;
}