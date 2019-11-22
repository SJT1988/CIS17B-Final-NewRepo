#pragma once
#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include "../AssetManager.h"
#include <map>
#include "Constants.h"

class SpriteComponent : public Component
{
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	

	bool animated = false;
	int numFrames = 0;
	int delay = 100; // milliseconds

public:
	SDL_Rect srcRect, destRect;
	std::string previousAnimation;
	int animIndex = 0;

	// create a mapping between indices and Animations
	std::map<const char*, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	SpriteComponent() = default;
	
	SpriteComponent(std::string texID)
	{
		setTexture(texID);
	}

	SpriteComponent(std::string textureID, bool isAnimated)
	{
		animated = isAnimated;

		// why are the player's animations initialized here?
		// the tutorial writer was in a hurry and didn't refactor.

		Animation player_idleUp = Animation(0, 1, 1); // IdleUp pose is index 0, 1 frame long, and has a 1 second delay
		Animation player_idleDown = Animation(1, 1, 1);
		Animation player_idleRight = Animation(2, 1, 1);

		Animation player_walkUp = Animation(3, 4, 100);
		Animation player_walkDown = Animation(4, 4, 100);
		Animation player_walkRight = Animation(5, 4, 100);

		Animation player_shootUp = Animation(6, 4, 100);
		Animation player_shootDown = Animation(7, 4, 100);
		Animation player_shootRight = Animation(8, 4, 100);

		Animation monster_walk = Animation(0, 4, 100);

		animations.emplace("IdleUp", player_idleUp);
		animations.emplace("IdleDown", player_idleDown);
		animations.emplace("IdleRight", player_idleRight);
		animations.emplace("WalkUp", player_walkUp);
		animations.emplace("WalkDown", player_walkDown);
		animations.emplace("WalkRight", player_walkRight);
		animations.emplace("ShootUp", player_shootUp);
		animations.emplace("ShootDown", player_shootDown);
		animations.emplace("ShootRight", player_shootRight);

		animations.emplace("MonsterWalk", monster_walk);
		
		// as of now, I am convinced this Play() function sets the animIndex to 1 for every object
		// we create that has a SpriteComponent. We need to find a way to not run this every time
		// presuming that is indeed what is happening.  I am especially woried (but have not yet
		// confirmed) that we are loading the player's animations into the animations mapping twice,
		// or perhaps (and more likely) making a copy of these animations unecessarily for objects
		// that don't need them.
		Play("IdleDown"); // this initializes the player's sprite indices

		setTexture(textureID);
	}

	~SpriteComponent()
	{
	}

	void setTexture(std::string texID)
	{
		texture = Game::assets->GetTexture(texID);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{

		// The player is animated, so frames will be read by
		// incrementing the "window" (source rectangle) horizontally
		// on the sprite sheet.
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / delay) % numFrames);
		}

		/* Multiple-frame animations will have their animations
		indexed by the y-pixel-position of the source rectangle's
		upper-left corner in the spritemap. In the character's
		case, that will be integral multiples of 2*TILE_SIZE (64px),
		eg. 0, 64, 128, etc.*/
		srcRect.y = animIndex * transform->height;

		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
		destRect.w = transform->width*transform->scale;
		destRect.h = transform->height*transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animationName)
	{
		numFrames = animations[animationName].frames;
		animIndex = animations[animationName].index;
		delay = animations[animationName].delay;
		previousAnimation = animationName;
	}
};