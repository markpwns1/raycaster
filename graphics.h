#pragma once

#include "stdafx.h"

using namespace sf;
using namespace std;

class Graphics
{
public:
	static const int WINDOW_WIDTH;
	static const int WINDOW_HEIGHT;

	static const int BUFFER_WIDTH;
	static const int BUFFER_HEIGHT;

	static const float BUFFER_CENTER_X;
	static const float BUFFER_CENTER_Y;

	static const float ASPECT_RATIO;

	static sf::Texture* workingTexture;
	static RenderTexture* buffer;

	static Color colour(float r, float g, float b, float a);
	static Color colour(float r, float g, float b);

	static void init();

	static void drawPixel(float x, float y, Color c);

	static void updateColumn(int x, float height, float sc, Color c);
	static void clearColumn(int x);
	static void drawColumns();

	static void setCurrentTexture(sf::Texture * tex);

	static void drawTexture(float x, float y, int w, int h, Shader * shader);

	static void flip(RenderWindow* window);

private:
	static Sprite* bufferSprite;
	static Sprite* textureSprite;
	static VertexArray line;
	static VertexArray pixel;
	static Vector2u workingTextureSize;
};

