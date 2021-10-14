
#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include "ShotComponent.h"

ShotComponent::ShotComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
	// todo
	//mOwner->GetGame()->AddShot(this);
}

ShotComponent::~ShotComponent()
{

}
