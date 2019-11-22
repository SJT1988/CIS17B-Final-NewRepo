#pragma once
#include "ECS.h"
#include "SDL.h"
#include "..\TextureManager.h"

class TileComponent : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int posX, int posY, int tileSize, int tileScale, std::string textureID)
	{
		texture = Game::assets->GetTexture(textureID);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tileSize;

		destRect.x = posX;
		destRect.y = posY;
		destRect.w = destRect.h = tileSize * tileScale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};