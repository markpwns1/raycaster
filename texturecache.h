#include "stdafx.h"

using namespace std;
using namespace sf;

#pragma once
class TextureCache
{
public:
	static map<string, Texture*> textures;

	static Texture* get(string name);
	static Texture load(string name);
	static Vector2u sizeOf(string name);
};

