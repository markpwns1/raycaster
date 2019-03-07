#include "stdafx.h"
#include "Graphics.h"
#include "TextureCache.h";

using namespace std;
using namespace sf;
using namespace maths;

sf::Texture* Graphics::workingTexture;

const int Graphics::WINDOW_WIDTH = 768;
const int Graphics::WINDOW_HEIGHT = 512;

const int Graphics::BUFFER_WIDTH = 192;
const int Graphics::BUFFER_HEIGHT = 128;

const float Graphics::ASPECT_RATIO = BUFFER_WIDTH / float(BUFFER_HEIGHT);

const float Graphics::BUFFER_CENTER_X = BUFFER_WIDTH / 2.0f;
const float Graphics::BUFFER_CENTER_Y = BUFFER_HEIGHT / 2.0f;

RenderTexture* Graphics::buffer;

Sprite* Graphics::bufferSprite;
Sprite* Graphics::textureSprite;

VertexArray Graphics::line(Lines, 2 * BUFFER_WIDTH);
VertexArray Graphics::pixel(Points, 1);
Vector2u Graphics::workingTextureSize;

Color Graphics::colour(float r, float g, float b, float a)
{
	Uint8 red = static_cast<Uint8>(limit(r, 0.0f, 1.0f) * 255);
	Uint8 green = static_cast<Uint8>(limit(g, 0.0f, 1.0f) * 255);
	Uint8 blue = static_cast<Uint8>(limit(b, 0.0f, 1.0f) * 255);
	Uint8 alpha = static_cast<Uint8>(limit(a, 0.0f, 1.0f) * 255);
	return Color(red, green, blue, alpha);
}

Color Graphics::colour(float r, float g, float b)
{
	Uint8 red = static_cast<Uint8>(limit(r, 0.0f, 1.0f) * 255);
	Uint8 green = static_cast<Uint8>(limit(g, 0.0f, 1.0f) * 255);
	Uint8 blue = static_cast<Uint8>(limit(b, 0.0f, 1.0f) * 255);
	return Color(red, green, blue, 255u);
}

void Graphics::init()
{
	buffer = new RenderTexture();
	buffer->create(BUFFER_WIDTH, BUFFER_HEIGHT);

	bufferSprite = new Sprite(buffer->getTexture());
	bufferSprite->setScale(WINDOW_WIDTH / (float)BUFFER_WIDTH, WINDOW_HEIGHT / (float)BUFFER_HEIGHT);
	//bufferSprite->setColor(Color::White);

	textureSprite = new Sprite();

	for (int i = 0; i < 2 * BUFFER_WIDTH; ++i) {
		line[i] = Vertex();
	}
	
	pixel[0] = Vertex();
}

// useful for debugging projections
void Graphics::drawPixel(float x, float y, Color c)
{
	pixel[0].position = Vector2f(x, y);
	pixel[0].color = c;
	buffer->draw(pixel);
}

void Graphics::drawTexture(float x, float y, int w, int h, Shader* shader)
{
	textureSprite->setPosition(x, y);
	textureSprite->setTextureRect(IntRect(0, 0, workingTextureSize.x, workingTextureSize.y));
	textureSprite->setScale(w / float(workingTextureSize.x), h / float(workingTextureSize.y));
	
	buffer->draw(*textureSprite, RenderStates(shader));
}

void Graphics::setCurrentTexture(sf::Texture* tex) {
	workingTexture = tex;
	textureSprite->setTexture(*tex);
	workingTextureSize = tex->getSize();
}

// 0 2 4 6 
// 1 3 5 7 
void Graphics::updateColumn(int x, float height, float sx, Color c)
{
	line[x * 2].position = Vector2f(x, Graphics::BUFFER_CENTER_Y - height / 2.0f);
	line[x * 2 + 1].position = Vector2f(x, Graphics::BUFFER_CENTER_Y + height / 2.0f);

	line[x * 2].texCoords = Vector2f(sx * workingTextureSize.x, 0.0f);
	line[x * 2 + 1].texCoords = Vector2f(sx * workingTextureSize.x, workingTextureSize.y);

	line[x * 2].color = c;
	line[x * 2 + 1].color = c;
}

void Graphics::clearColumn(int x) {
	line[x * 2].color = Color::Transparent;
	line[x * 2 + 1].color = Color::Transparent;
}

void Graphics::drawColumns() {
	buffer->draw(line, Graphics::workingTexture);
}

void Graphics::flip(RenderWindow* window) 
{
	//bufferSprite->setTexture(buffer->getTexture());
	window->draw(*bufferSprite);
	window->display();
	//buffer->clear();
}
