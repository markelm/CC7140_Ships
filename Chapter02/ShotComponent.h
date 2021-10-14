#pragma once
#include "Component.h"

class ShotComponent :
    public Component
{
public:
    ShotComponent(class Actor* owner, int updateOrder);
    ~ShotComponent();

protected:
    
};

