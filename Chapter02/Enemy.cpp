#include "Enemy.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Enemy::Enemy(Game* game)
	:Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
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
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Vector2 pos = GetPosition();

	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;

	float wWidth = GetGame()->GetWindowWidth();
	float wHeight = GetGame()->GetWindowHeight();

	if (pos.x < 0.0f)
	{
		pos.x = 0.0f;
	}
	else if (pos.x > wWidth)
	{
		// todo: get Actor width and 
		// subtract it 
		pos.x = wWidth;
	}

	if (pos.y < 0.0f)
	{
		pos.y = 0.0f;
	}
	else if (pos.y > wHeight)
	{
		// todo: get Actor height and 
		// subtract it 
		pos.y = wHeight;
	}

	SetPosition(pos);
}
