/*

This class was modified from this version: https://gist.github.com/kenpower/7233967

HOW IT WORKS:
- Every frame, FPSCounter::frames is incremented
- At the end of every second, the window title is set to FPSCounter::frames, which
  should equal the amount of frames that ran that second.

*/

#pragma once

#include "stdafx.h"

class FPSCounter
{
private:
	static unsigned int frames;
	static sf::Clock clock;

public:
	static RenderWindow* window;
	static void count()
	{
		if (clock.getElapsedTime().asSeconds() >= 1.0f)
		{
			window->setTitle("FPS: " + to_string(frames));

			frames = 0;
			clock.restart();

		}

		++frames;
	}
};

// Since fpscounter.h is only ever included in main.cpp, these won't cause a multiple declaration error
RenderWindow* FPSCounter::window;
unsigned int FPSCounter::frames;
sf::Clock FPSCounter::clock;