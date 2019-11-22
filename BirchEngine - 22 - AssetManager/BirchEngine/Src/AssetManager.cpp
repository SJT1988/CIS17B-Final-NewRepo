#include "AssetManager.h"
#include "ECS\Components.h"

AssetManager::AssetManager(Manager * man) : manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int rng, int sp, std::string texID)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, TILE_SIZE, TILE_SIZE, 1);
	projectile.addComponent<SpriteComponent>(texID, false);
	projectile.getComponent<SpriteComponent>().animIndex = 0;
	projectile.addComponent<ProjectileComponent>(rng, sp, vel);
	projectile.addComponent<ColliderComponent>("projectile", 13, 13, 6, 6);
	projectile.addGroup(Game::groupProjectiles);

}

void AssetManager::AddTexture(std::string id, const char * path)
{
	// Load texture will return an SDL pointer to the texture file
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture * AssetManager::GetTexture(std::string id)
{
	return textures[id];
}
