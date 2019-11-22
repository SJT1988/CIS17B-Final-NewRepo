#pragma once
#include <string>
#include "Game.h"

class Map
{
public:
	Map(std::string texID, int mMapScale, int mTileSize);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY, enum Game::groupLabels groupLabel);
	void LoadColliders(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int posX, int posY, enum Game::groupLabels groupLabel);

private:

	std::string textureID;
	int mapScale;
	int tileSize;
	int scaledSize;
};