
#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include "Projectile.h"
#include "ShootComponent.h"

template<class P>
ShootComponent<P>::ShootComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
	// todo
	//mOwner->GetGame()->AddShot(this);
}

template<class P>
ShootComponent<P>::~ShootComponent()
{

}

template<class P>
void ShootComponent<P>::Shoot()
{
	// Create ship's projectile
	P *p = new P(mOwner->GetGame());

	Vector2 ship_pos = mOwner->GetPosition();

	p->SetPosition(Vector2(ship_pos));
	
	//p->SetScale(1.5f);
}
