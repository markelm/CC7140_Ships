#pragma once
#include "Component.h"

class ShootComponent :
    public Component
{
public:
    ShootComponent(class Actor* owner, int updateOrder = 100);
    ~ShootComponent();

    virtual void Shoot();

protected:
    
};

