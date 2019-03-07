#pragma once

#include "stdafx.h"
#include "Wall.h"

using namespace std;

class World
{
public:
	static const float WALL_HEIGHT;
	static const float CEILING_HEIGHT;

	static Vector2f spawnPoint;

	static vector<Wall*> walls;
	static int wallCount;

	static vector<Wall*> workedWalls;
	static int workedWallCount;

	static void loadFromFile(string filename);
	static void cullWalls();
};

