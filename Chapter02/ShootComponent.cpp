
#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include "Projectile.h"
#include "ShootComponent.h"

ShootComponent::ShootComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
	// todo
	//mOwner->GetGame()->AddShot(this);
}

ShootComponent::~ShootComponent()
{

}

void ShootComponent::Shoot()
{
	// Create ship's projectile
	Projectile *p = new Projectile(mOwner->GetGame());

	Vector2 ship_pos = mOwner->GetPosition();

	p->SetPosition(Vector2(ship_pos));
	
	p->SetScale(1.5f);
}
