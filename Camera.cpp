#include "stdafx.h"
#include "camera.h"
#include "graphics.h"
#include "ray.h"
#include "world.h"
#include "texturecache.h"
#include "rayhit.h"
#include <memory>
#include "floorceiling.h"
#include <ppl.h>

using namespace maths;

const float Camera::HEIGHT = 1.0f;

const float Camera::MOVE_SPEED = 5.0f;
const float Camera::ROTATION_SPEED = rad(180.0f);

const float Camera::VIEW_DISTANCE = 8.0f;
const float Camera::FOG_DISTANCE = 8.0f;

const float Camera::FOV_H = rad(60.0f);
float Camera::FOV_V;

float Camera::rotation = 1.0f;
Vector2f Camera::forward = vec_from_rot(rotation);

Vector2f Camera::position;
Shader Camera::floorCeilingShader;

void Camera::init()
{
	// Because spawnpoint is decided upon loading the world, Camera::init() must be called after World::loadFromFile(path)
	position = World::spawnPoint;

	// Because FOV_V depends on Graphics::ASPECT_RATIO, Camera::init() must be called after Graphics::init()
	FOV_V = FOV_H / Graphics::ASPECT_RATIO;

	/*if (!floorCeilingShader.isAvailable()) {
		throw "Shaders are not supported on this system.";
		exit(0);
	}

	floorCeilingShader.loadFromFile("assets/floor_ceiling.frag", Shader::Fragment);

	floorCeilingShader.setUniform("fov_h", FOV_H);
	floorCeilingShader.setUniform("fov_v", FOV_V);

	floorCeilingShader.setUniform("fog_distance", FOG_DISTANCE);

	floorCeilingShader.setUniform("floor_texture", *TextureCache::get("floor.png"));
	floorCeilingShader.setUniform("ceiling_texture", *TextureCache::get("ceiling.png"));*/

	FloorCeiling::init();
}

void Camera::update(float dt)
{
	rotation -= Keyboard::isKeyPressed(Keyboard::Right) * ROTATION_SPEED * dt;
	rotation += Keyboard::isKeyPressed(Keyboard::Left) * ROTATION_SPEED * dt;
	
	forward = vec_from_rot(rotation);

	position += forward * (float)Keyboard::isKeyPressed(Keyboard::W) * MOVE_SPEED * dt;
	position -= forward * (float)Keyboard::isKeyPressed(Keyboard::S) * MOVE_SPEED * dt;
	
	position += vec_perp(forward) * (float)Keyboard::isKeyPressed(Keyboard::A) * MOVE_SPEED * dt;
	position -= vec_perp(forward) * (float)Keyboard::isKeyPressed(Keyboard::D) * MOVE_SPEED * dt;
	
	//floorCeilingShader.setUniform("camera_pos", position);
	//floorCeilingShader.setUniform("camera_rot", rotation);
	FloorCeiling::update();
}

void Camera::render()
{
	//Graphics::drawTexture(0, Graphics::BUFFER_HEIGHT, Graphics::BUFFER_WIDTH, -Graphics::BUFFER_HEIGHT, &floorCeilingShader);
	FloorCeiling::render();

	// Re-using variables gives a slight FPS increase
	// Adds up a lot when it's repeated 192 times
	float angleX, depth, height, darkness;
	Color c;
	Ray::origin = position;

	for (int screenX = 0; screenX < Graphics::BUFFER_WIDTH; ++screenX)
	{
		angleX = (0.5f - screenX / float(Graphics::BUFFER_WIDTH)) * FOV_H;

		Ray::direction = vec_from_rot(angleX + rotation);
		Ray::cast();

		if (!Ray::hit || Ray::hit_distance > VIEW_DISTANCE) 
		{
			Graphics::clearColumn(screenX);
			continue;
		}

		depth = Ray::hit_distance * cosf(angleX);  

		height = Graphics::BUFFER_HEIGHT * (World::WALL_HEIGHT / depth);

		darkness = Ray::hit_distance / FOG_DISTANCE;

		c = Graphics::colour(1.0f - darkness, 1.0f - darkness, 1.0f - darkness); // bottleneck?
		
		Graphics::updateColumn(screenX, height, Ray::hit_textureX, c);
	}

	Graphics::drawColumns();
}
