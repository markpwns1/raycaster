#pragma once

#include "stdafx.h"

using namespace sf;

class Camera
{
public:
	static const float HEIGHT;

	static const float MOVE_SPEED;
	static const float ROTATION_SPEED;

	static const float VIEW_DISTANCE;
	static const float FOG_DISTANCE;

	static const float FOV_H;
	static float FOV_V;

	static float rotation;

	static Vector2f position;
	static Vector2f forward;

	static Shader floorCeilingShader;

	static void init();

	static void update(float dt);
	static void render();
};

