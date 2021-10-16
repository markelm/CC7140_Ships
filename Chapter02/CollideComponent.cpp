#include "CollideComponent.h"
#include "Ship.h"
#include "Enemy.h"

#include "Game.h"

CollideComponent::CollideComponent(class Actor* owner, int updateOrder)
	: Component(owner, updateOrder)
{
	mOwner->GetGame()->AddCollider(this);

}
CollideComponent::~CollideComponent() 
{
	mOwner->GetGame()->RemoveCollider(this);
}

bool CollideComponent::Collide(Actor* other) 
{

	if (other == mOwner) {
		return false;
	}

	Vector2 otherPos = other->GetPosition();
	float otherHeight = other->GetHeight();
	float otherWidth = other->GetWidth();

	Vector2 ownerPos = mOwner->GetPosition();
	float ownerHeight = mOwner->GetHeight();
	float ownerWidth = mOwner->GetHeight();

	return ((ownerPos.x < otherPos.x + otherWidth / 2.0f &&
		otherPos.x < ownerPos.x + ownerWidth / 2.0f) &&
		(ownerPos.y < otherPos.y + otherHeight / 2.0f &&
			otherPos.y < ownerPos.y + otherHeight / 2.0f));

}
