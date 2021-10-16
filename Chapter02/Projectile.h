#pragma once
#include "Actor.h"
class Projectile :
    public Actor
{
public:
    Projectile(class Game* game);
    void UpdateActor(float deltaTime) override;
    float GetSpeed() const { return speed; }

private:
    float speed;
};

