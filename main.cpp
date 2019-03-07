#include "stdafx.h"
#include "graphics.h"
#include "world.h"
#include "camera.h"
#include "maths.h"
#include "texturecache.h"
#include "fpscounter.h"

using namespace std;
using namespace maths;

const int MIN_ACCEPTABLE_FRAMERATE = 60;
const bool LIMIT_FPS = false; // Whether or not to limit FPS to MIN_ACCEPTABLE_FRAMERATE

RenderWindow* window;
Event event;

void draw() 
{
	// Camera renders everything to a buffer
	// All rendering is handled by the Graphics class
	Camera::render();
	
	// After rendering to the buffer, draw the buffer onto the window
	Graphics::flip(window);
}

// dt is the time that passed since the last frame.
// Math that needs to be framerate independent should use dt
void update(float dt)
{
	// Handles stuff like controls
	Camera::update(dt);

	// Based on stuff like position, rotation, tries to narrow down which
	// walls a ray could intersect with
	World::cullWalls();
}

const float LOGIC_TIMESTEP = 1.0f / MIN_ACCEPTABLE_FRAMERATE;

int main()
{
	// See world.cpp
	World::loadFromFile("assets/world.txt");

	// cxt is passed as an argument in the RenderWindow constructor
	// To ensure maximum performance, every setting is disabled
	// Enabling them doesn't even do anything
	ContextSettings cxt;
	cxt.antialiasingLevel = 0;
	cxt.depthBits = 0;
	cxt.stencilBits = 0;
	cxt.sRgbCapable = false;

	// See graphics.cpp and camera.cpp
	Graphics::init();

	// Since changing textures is an expensive operation, the Graphics class re-uses
	// the same texture until it's time to use a different one
	Graphics::setCurrentTexture(TextureCache::get(Wall::DEFAULT_TEXTURE));

	Camera::init();

	window = new RenderWindow(sf::VideoMode(Graphics::WINDOW_WIDTH, Graphics::WINDOW_HEIGHT), "Raycaster written in C++ and SFML 2.0", Style::Default, cxt);

	FPSCounter::window = window;

	// Booleans can be cast to unsigned ints in C++
	window->setFramerateLimit(LIMIT_FPS * MIN_ACCEPTABLE_FRAMERATE);

	// The game loop separates rendering and logic (kind of)
	// Logic is capped at MIN_ACCEPTABLE_FRAMERATE ticks per second
	// Rendering is done as frequently as possible, unless LIMIT_FPS == true
	// For further reading: https://gafferongames.com/post/fix_your_timestep/
	Clock frameTime;
	float deltaTime = 0.0f;

	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window->close();
			}
		}

		draw();

		// This makes it so that update() is only called every few frames
		deltaTime += frameTime.restart().asSeconds();

		// If FPS is already capped at 60, update every frame, otherwise update once
		// every few frames
		if(LIMIT_FPS || deltaTime >= LOGIC_TIMESTEP) {
			update(deltaTime);
			deltaTime = 0.0f;
		}

		FPSCounter::count();
	}

	return 0;
}