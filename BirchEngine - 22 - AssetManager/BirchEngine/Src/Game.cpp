#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "Constants.h"
#include <cstdlib>
#include <ctime>

Map* sceneMap;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
//auto& monster(manager.addEntity());

Vector2D playerPosition;

// put tiles in the game:

Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	srand(static_cast<unsigned>(time(NULL)));

	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	assets->AddTexture("terrain", "Assets/tileset.png");
	assets->AddTexture("player", "Assets/RickTangle_SpriteSheet.png");
	assets->AddTexture("projectile", "Assets/bullet.png");
	assets->AddTexture("monster", "Assets/monster.png");
	// assets->AddTexture("collider", "Assets/collider.png");
	sceneMap = new Map("terrain", 1, TILE_SIZE);

	// +----------------------------+
	// | $$$ ECS IMPLEMENTATION $$$ |
	// +----------------------------+

	// background map:
	sceneMap->LoadMap("Assets/map01BG.map", 11, 11, Game::groupMapBG);
	// 'the' map:
	sceneMap->LoadMap("Assets/map01.map", 11, 11, Game::groupMap);
	// transform coordinates are in pixels. Player instantiated at (0,0) by default.
	// Because the player sprites are 64x64 but the upper left of his body is 16 over, 16, down,
	// we need to adjust for the offset when we place him:
	player.addComponent<TransformComponent>(5 * TILE_SIZE - 16, 2 * TILE_SIZE - 16, Vector2D(0,1), 64, 64, 1);  // (5 * TILE_SIZE, 2 * TILE_SIZE); 
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player", 16, 16, TILE_SIZE, TILE_SIZE);
	player.addGroup(groupPlayers); // reminder: player(s) is/are being drawn in Update()

	
	playerPosition = player.getComponent<TransformComponent>().position;


	//makes spiders of random size from 50% to 150% scale
	for (int i = 0; i < 3; i++)	{
		float temp = .2 + (static_cast<float>(rand())) / (static_cast<float>(RAND_MAX / (1.5 - .2)));
		assets->CreateSpider(rand()% - 200, rand() % 100, temp);
	}

	

	// fx map/overlays:
	sceneMap->LoadMap("Assets/map01FX.map", 11, 11, groupMapFX);

	// load colliders
	sceneMap->Map::LoadColliders("Assets/map01Colliders.map", 11, 11);
}

auto& mapBgTiles(manager.getGroup(Game::groupMapBG));
auto& mapTiles(manager.getGroup(Game::groupMap));
auto& mapFxTiles(manager.getGroup(Game::groupMapFX));
auto& players(manager.getGroup(Game::groupPlayers));
auto& monsters(manager.getGroup(Game::groupMonsters));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	srand(static_cast<unsigned>(time(NULL)));

	manager.refresh();
	manager.update();

	bool setPlayerPos = true;
	SDL_Rect playerCollider = player.getComponent<ColliderComponent>().collider;
	for (auto& c : colliders)
	{
		SDL_Rect cCollider = c->getComponent<ColliderComponent>().collider;
		if (c->getComponent<ColliderComponent>().tag == "terrainCollider" &&
			Collision::AABB(cCollider, playerCollider))
		{
			setPlayerPos = false;
			break;
		}
	}
	if (setPlayerPos == true)
	{
		playerPosition = player.getComponent<TransformComponent>().position;
	}
	
	// handle player collision with the map
	for (auto& c : colliders)
	{
		SDL_Rect cCollider = c->getComponent<ColliderComponent>().collider;
		if (c->getComponent<ColliderComponent>().tag == "terrainCollider" &&
			Collision::AABB(cCollider, playerCollider))
		{
			// if player collides, he is reset to previous position he was in
			player.getComponent<TransformComponent>().position = playerPosition;
			std::cout << "Try not to stub your precious little toes..." << std::endl;
		}
	}

	
	for (auto& m : monsters)
	{
		float speedLo = m->getComponent<TransformComponent>().speedLo;
		float speedHi = m->getComponent<TransformComponent>().speedHi;
		
		//jitters the speed
		m->getComponent<TransformComponent>().speed =
			speedLo + (static_cast<float>(rand())) /
			(static_cast<float>(RAND_MAX / (speedHi - speedLo)));

		SDL_Rect mCollider = m->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(mCollider, playerCollider))
		{
			// We probably want the spiders to be able to overlap player
			std::cout << "Don't get up in that spider's business!" << std::endl;
		}

		//movement of enemies
		//simple tracking algorithm
		// hunter velocity changes based on the player's relative position
		//if player is to the U/D/L/R, move U/D/L/R
		if (player.getComponent<TransformComponent>().position.x <
			m->getComponent<TransformComponent>().position.x) {
			m->getComponent<TransformComponent>().velocity.x = -1;
		} else {
				m->getComponent<TransformComponent>().velocity.x = 1;
		}

		if (player.getComponent<TransformComponent>().position.y <
			m->getComponent<TransformComponent>().position.y){
				m->getComponent<TransformComponent>().velocity.y = - 1;
		} else {
				m->getComponent<TransformComponent>().velocity.y = 1;
		}



	}

	// handle projectile collsions
	for (auto& p : projectiles)
	{
		for (auto& m : monsters)
		{
			if (Collision::AABB(m->getComponent<ColliderComponent>().collider,
				p->getComponent<ColliderComponent>().collider))
			{
				p->destroy();
				m->destroy();
				std::cout << "You shot a spider!" << std::endl;
			}
		}
		for (auto& c : colliders)
		{
			SDL_Rect cCollider = c->getComponent<ColliderComponent>().collider;
			if ((c->getComponent<ColliderComponent>().tag == "terrainCollider") &&
				Collision::AABB(cCollider, playerCollider))
			{
				p->destroy();
				std::cout << "Nice shot." << std::endl;
			}
		}
	}
}

void Game::render()
{
	//start with this
	SDL_RenderClear(renderer);
	
	//first draw all the tiles:
	for (auto& t : mapBgTiles)
	{
		t->draw();
	}
	for (auto& t : mapTiles)
	{
		t->draw();
	}
	// DEBUG ONLY:
	// This line must be uncommented to see terrain colliders, specifically
	// Those colliders have the tag "terrainCollider"
	/*
	for (auto& c : colliders)
	{
		c->draw();
	}
	*/
	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& m : monsters)
	{
		m->draw();
	}
	for (auto& t : mapFxTiles)
	{
		t->draw();
	}
	//end with this
	// std::cout << "(" << players[0]->getComponent<SpriteComponent>().srcRect.x << ", " << players[0]->getComponent<SpriteComponent>().srcRect.y << ")" << std::endl;
	// std::cout << projectiles[0]->getComponent<SpriteComponent>().animIndex << std::endl;
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
