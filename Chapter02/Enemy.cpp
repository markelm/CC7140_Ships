#include "Enemy.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Enemy::Enemy(Game* game, bool timeWarp, float speed)
	:Projectile(game, timeWarp, speed)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Enemy01.png"),
		game->GetTexture("Assets/Enemy02.png"),
		game->GetTexture("Assets/Enemy03.png"),
		game->GetTexture("Assets/Enemy04.png"),
		game->GetTexture("Assets/Enemy05.png"),
		game->GetTexture("Assets/Enemy06.png")
	};

	asc->SetAnimTextures(anims);

	mWidth = asc->GetTexWidth();
	mHeight = asc->GetTexHeight();
}

void Enemy::UpdateActor(float deltaTime)
{
	Projectile::UpdateActor(deltaTime);
}
