#pragma once
#include "Actor.h"
class Projectile :
    public Actor
{
public:
    Projectile(class Game* game, float speed = 300.0f);
    void UpdateActor(float deltaTime) override;
    float GetSpeed() const { return mSpeed; }

private:
    float mSpeed;
};

