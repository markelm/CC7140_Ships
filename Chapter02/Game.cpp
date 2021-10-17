// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#define BUFFSIZE 1024

#include <algorithm>
#include "SDL/SDL_image.h"

#include "Actor.h"
#include "BGSpriteComponent.h"
#include "Enemy.h"
#include "Game.h"
#include "Ship.h"
#include "ShootComponent.h"
#include "SpriteComponent.h"
#include "TextComponent.h"

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mWindowWidth(920)
	, mWindowHeight(460)
	, mTicksCount(0)
	, mShip(nullptr)
	, mCoolDown(0)
	, mScore(0)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 
		100, 100, 
		GetWindowWidth(), GetWindowHeight(), 
		0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	mCoolDown = 5;
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Process ship input
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		float newDelta = deltaTime;
		if (GetTimeWarp() && actor->IsAffectedByTimeWarp()) {
			newDelta = deltaTime * 0.1f;
		}
		actor->Update(newDelta);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	auto& spawner = mSpawners.front();
	if (mCoolDown < 1)
	{
		mCoolDown = 200;

		spawner->Shoot();
		mSpawners.push(spawner);
		mSpawners.pop();
	}

	if (mCoolDown > 0) mCoolDown -= 1;

	for (auto collider : mColliders) {
		for (auto& actor : mActors) {

			if (std::find_if(mColliders.begin(), mColliders.end(), 
				[&](CollideComponent* c) -> bool { return c->GetOwner() == actor; }) != mColliders.end() && 
				(collider->Collide(actor))) {
				actor->SetState(Actor::State::EDead);
			}
		}
	}

	mText->SetText(mRenderer, "\nMissed Alien Ships: %3d", mScore);
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	
	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);

	
}

void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	mFont = TTF_OpenFont("VT323-Regular.ttf", 24);


	Actor* top = new Actor(this);
	top->SetPosition(Vector2(GetWindowWidth(), GetWindowHeight() / 4.0f));
	
	Actor* middle = new Actor(this);
	middle->SetPosition(Vector2(GetWindowWidth(), GetWindowHeight() / 2.0f));

	Actor* bottom = new Actor(this);
	bottom->SetPosition(Vector2(GetWindowWidth(), 3.0f * GetWindowHeight() / 4.0f));

	// Create enemy spawners
	mSpawners.push(new ShootComponent<Enemy>(top));
	mSpawners.push(new ShootComponent<Enemy>(middle));
	mSpawners.push(new ShootComponent<Enemy>(bottom));

	//--------------------------------Criação do background----------------------------
	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(GetWindowWidth() / 2.0f , GetWindowHeight() / 2.0f));
	
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(GetWindowWidth(), GetWindowHeight()));//set the screen size for the background to fit in
	//create a vector
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	//set this vector to the background component
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(GetWindowWidth(), GetWindowHeight()));//set the screen size for the background to fit in
	//create a vector
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	//set this vector to the background component
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	mText = new TextComponent(temp);
	mText->SetText(mRenderer, "");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

// =load the textures
SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

//add actors
void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

//remove any actors
void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

//add sprites
void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for ( ;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

//remove sprite
void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::AddCollider(CollideComponent* collider)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myUpdateOrder = collider->GetUpdateOrder();
	auto iter = mColliders.begin();
	for (;
		iter != mColliders.end();
		++iter)
	{
		if (myUpdateOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mColliders.insert(iter, collider);
}

//remove sprite
void Game::RemoveCollider(CollideComponent* collider)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
	mColliders.erase(iter);
}

void Game::DrawText(const char* fmt, ...) {
	char buffer[BUFFSIZE];

	va_list rest;
	va_start(rest, fmt);

	SDL_vsnprintf(buffer, BUFFSIZE, fmt, rest);

	va_end(rest);

	SDL_Color color = {
		255, 255, 255, 255
	};

	SDL_Surface* text = TTF_RenderText_Solid(mFont, buffer, color);

	SDL_Texture* textureText = SDL_CreateTextureFromSurface(mRenderer, text);

	SDL_Rect dest = { 0 };

	SDL_QueryTexture(textureText, NULL, NULL, &dest.w, &dest.h);

	SDL_FreeSurface(text);

	dest.x = GetWindowWidth()/2.0f;
	dest.y = GetWindowHeight()/2.0f;

	SDL_RenderCopy(mRenderer, textureText, NULL, &dest);

	SDL_DestroyTexture(textureText);
}
