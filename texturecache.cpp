#include "stdafx.h"
#include "TextureCache.h"

map<string, Texture*> TextureCache::textures;

Texture* TextureCache::get(string name)
{
	if (textures.count(name) < 1) {
		Texture* tex = new Texture();
		if (!tex->loadFromFile("assets/" + name)) {
			throw invalid_argument("Texture does not exist: assets/" + name);
		}
		textures.insert(pair<string, Texture*>(name, tex));
	}

	return textures[name];
}

Texture TextureCache::load(string name)
{
	Texture tex;
	if (!tex.loadFromFile(name)) {
		throw invalid_argument("Texture does not exist: " + name);
	}
	return tex;
}

Vector2u TextureCache::sizeOf(string name)
{
	return get(name)->getSize();
}
