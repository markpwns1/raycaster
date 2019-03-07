#pragma once

#include <SFML\System.hpp>

using namespace sf;
using namespace std;

class Wall
{
public:
	static const string DEFAULT_TEXTURE;

	Vector2f pointA;
	Vector2f pointB;

	Wall(Vector2f p1, Vector2f p2);
};