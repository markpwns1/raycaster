#pragma once

#include "stdafx.h"

class FloorCeiling {
public:
	static const string SHADER_FILENAME;
	static const string FALLBACK_TEXTURE_FILENAME;

	static Shader shader;
	static Texture fallbackTexture;
	static Sprite fallbackTextureSprite;

	static void init();
	static void update();
	static void render();
private:
	static bool useFallbackTexture;

};