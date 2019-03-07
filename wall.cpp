#include "stdafx.h"
#include "Wall.h"

using namespace sf;

const string Wall::DEFAULT_TEXTURE = "wall.png";

Wall::Wall(Vector2f p1, Vector2f p2)
{
	this->pointA = p1;
	this->pointB = p2;
}