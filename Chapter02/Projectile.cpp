#include "Projectile.h"
#include "AnimSpriteComponent.h"
#include "Game.h"


Projectile::Projectile(Game* game, float speed)
	:Actor(game),
	mSpeed(speed)

{
	SpriteComponent* sc = new SpriteComponent(this);
	SDL_Texture* texture = game->GetTexture("Assets/Laser.png");
	sc->SetTexture(texture);
}

void Projectile::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();

	pos.x += mSpeed * deltaTime;

	if (pos.x > GetGame()->GetWindowWidth()) {
		SetState(State::EDead);
	}

	SetPosition(pos);
}