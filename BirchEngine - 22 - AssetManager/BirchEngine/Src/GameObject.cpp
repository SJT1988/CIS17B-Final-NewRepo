#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char * textureSheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(textureSheet);
	posX = x;
	posY = y;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	posX++;
	posY++;

	srcRect.w = 32;
	srcRect.h = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = posX;
	destRect.y = posY;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}
