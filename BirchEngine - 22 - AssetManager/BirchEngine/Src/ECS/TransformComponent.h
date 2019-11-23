#pragma once
#include "Components.h"
#include "../Vector2D.h"
#include "Constants.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;
	Vector2D facing;

	int height = TILE_SIZE;
	int width = TILE_SIZE;
	float scale = 1;


	float speed = 3;
	float speedLo = NULL;
	float speedHi = NULL;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float sc)
	{
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	// overrides position, dimensions and scale
	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	TransformComponent(float x, float y, Vector2D direction, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		facing.x = direction.x;
		facing.y = direction.y;
		height = h;
		width = w;
		scale = sc;
	}


	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		float norm = velocity.Norm(); // std::sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
		position.x += (norm != 0) ? static_cast<int>((velocity.x * speed) / norm) : static_cast<int>(velocity.x * speed);
		position.y += (norm != 0) ? static_cast<int>((velocity.y * speed) / norm) : static_cast<int>(velocity.y * speed);
	}
};