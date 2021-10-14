#include "Projectile.h"
#include "AnimSpriteComponent.h"
#include "Game.h"


Projectile::Projectile(Game* game)
	:Actor(game),
	speed(300.0f)

{
	SpriteComponent* sc = new SpriteComponent(this);
	SDL_Texture* texture = game->GetTexture("Assests/Laser.png");
	sc->SetTexture(texture);
}

void Projectile::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();

	pos.x += speed * deltaTime;

	if (pos.x > GetGame()->GetWindowWidth()) {
		SetState(State::EDead);
	}



}