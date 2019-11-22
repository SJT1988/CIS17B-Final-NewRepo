#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	/*
	Constructor for a Projectile Component.
		rng:	How far a projectile can go before it is destroyed.
		sp:		The speed of the projectile.
	*/
	ProjectileComponent(int rng, int sp, Vector2D vel) 
	{
		this->range = rng;
		this->speed = sp;
		this->velocity = vel;
	}

	~ProjectileComponent() {};

	void init() override 
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}
	void update() override
	{
		distance += speed;

		// in game.cpp, projectiles are destroyed when they collide. But if
		// the projectile collides with nothing, it will be destroyed based
		// on its distance traveled and position coordinates.
		
		if (distance > range)
		{
			std::cout << "Projectile out of range!" << std::endl;
			entity->destroy();
		}
		else if (transform->position.x > 352 ||
			transform->position.x < 0  ||
			transform->position.y > 352  ||
			transform->position.y < 0 
			)
		{
			std::cout << "Projectile out of bounds!" << std::endl;
			entity->destroy();
		}
	}
private:
	TransformComponent* transform;

	// how far the projectile can go before being destroyed
	int range = 0;
	// speed of the projectile
	int speed = 0;
	// counter for how far the projectile has travelled
	int distance = 0;
	Vector2D velocity;
};