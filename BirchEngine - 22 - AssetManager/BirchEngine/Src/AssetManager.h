#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS\ECS.h"

class AssetManager
{
public:
	Manager * manager;
	AssetManager(Manager* man);
	~AssetManager();

	// Game Objects

	/*
	Arguments:
		pos := position
		rng := range
		sp := speed
		texID := textureID for projectile's texture
	*/
	void CreateProjectile(Vector2D pos, Vector2D vel, int rng, int sp, std::string texID);

	// Texture Management
	void AddTexture(std::string id, const char * path);
	SDL_Texture * GetTexture(std::string id);

private:
	// Manager * manager;
	// associate textures with id:
	std::map<std::string, SDL_Texture*> textures;
	
};