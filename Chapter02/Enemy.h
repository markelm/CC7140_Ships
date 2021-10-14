#pragma once
#include "Actor.h"
class Enemy :
    public Actor
{
public:
    Enemy(class Game* game);
    void UpdateActor(float deltaTime) override;
    float GetRightSpeed() const { return mRightSpeed; }
    float GetDownSpeed() const { return mDownSpeed; }
    
private:
    float mRightSpeed;
    float mDownSpeed;
};

