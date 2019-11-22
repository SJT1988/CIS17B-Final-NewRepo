#pragma once
#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
private:
	unsigned int currentTime;
	unsigned int lastTime = 0;
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				transform->facing = Vector2D(0, -1); // up
				sprite->Play("WalkUp");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				transform->facing = Vector2D(0, 1); // down
				sprite->Play("WalkDown");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				transform->facing = Vector2D(-1, 0); // left
				sprite->Play("WalkRight");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				transform->facing = Vector2D(1, 0); // right
				sprite->Play("WalkRight");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->Play("IdleUp");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->Play("IdleDown");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->Play("IdleRight");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->Play("IdleRight");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_MOUSEBUTTONDOWN)
		{
			currentTime = SDL_GetTicks();
			if (currentTime > lastTime + 500)
			{
				if (transform->facing == Vector2D(0, -1))
				{
					transform->velocity.Zero();
					sprite->Play("ShootUp");
					sprite->spriteFlip = SDL_FLIP_NONE;
					Game::assets->CreateProjectile(Vector2D(26, 16) + transform->position, Vector2D(0, -2), 352, 1, "projectile");
					currentTime = SDL_GetTicks();
					// fix repeating animation later
				}
				else if (transform->facing == Vector2D(0, 1))
				{
					transform->velocity.Zero();
					sprite->Play("ShootDown");
					sprite->spriteFlip = SDL_FLIP_NONE;
					Game::assets->CreateProjectile(Vector2D(5, 16) + transform->position, Vector2D(0, 2), 352, 1, "projectile");
					// fix repeating animation later
				}
				else if (transform->facing == Vector2D(1, 0))
				{
					transform->velocity.Zero();
					sprite->Play("ShootRight");
					Game::assets->CreateProjectile(Vector2D(32, 16) + (transform->position),
						Vector2D(2, 0), 352, 1, "projectile");
					// fix repeating animation later
				}
				else if (transform->facing == Vector2D(-1, 0))
				{
					transform->velocity.Zero();
					sprite->Play("ShootRight");
					Game::assets->CreateProjectile(Vector2D(-32, 16) + (transform->position),
						Vector2D(-2, 0), 352, 1, "projectile");
				}
				lastTime = currentTime;
			}
		}
	}
};