#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"

extern Manager manager; // manager is now the same variable as manager in Game.cpp

Map::Map(std::string texID, int mMapScale, int mTileSize) // : mapFilePath(mapFilePath), mapScale(mapScale), tileSize(tileSize)
{
	this->textureID = texID;
	this->mapScale = mMapScale;
	this->tileSize = mTileSize;

	scaledSize = mapScale * tileSize;
}

Map::~Map()
{
	
}

// Load the map tiles:
void Map::LoadMap(std::string path, int sizeX, int sizeY, enum Game::groupLabels groupLabel)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	// these loops parse the .map file:
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c)*tileSize; // atoi := character string-to-int
			mapFile.get(c);
			srcX = atoi(&c)*tileSize;
			AddTile(srcX, srcY, x * (scaledSize), y * (scaledSize), groupLabel);
			mapFile.ignore(); //skip the comma
		}
	}
	mapFile.close();
}

/*
TODO: This can still be refactored because right now we need to pass a specific Map object,
to run this function, but the tileset (.png) associated with it is worthless there; we only
need it here to initialize "scaledSize", which should be set for every map in the game.
*/
void Map::LoadColliders(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY; // these don't seem to be FOR anything...

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '1')
			{
				auto& tileCollider(manager.addEntity());
				tileCollider.addComponent<ColliderComponent>("terrainCollider", x * scaledSize, y * scaledSize, scaledSize, scaledSize);
				tileCollider.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int posX, int posY, enum Game::groupLabels groupLabel)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, posX, posY, tileSize, mapScale, textureID);
	tile.addGroup(groupLabel);
}

