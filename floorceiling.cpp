/*

This class handles the floor and ceiling of the raycaster engine. By default
the floor and ceiling are done with a fragment shader, but if that's not
possible, it just draws a fallback texture to the screen

*/

#include "stdafx.h"

#include "floorceiling.h"
#include "texturecache.h"
#include "graphics.h"
#include "camera.h"

const string FloorCeiling::SHADER_FILENAME = "assets/floor_ceiling.frag";
const string FloorCeiling::FALLBACK_TEXTURE_FILENAME = "assets/fallback.png";

Shader FloorCeiling::shader;
Texture FloorCeiling::fallbackTexture;
Sprite FloorCeiling::fallbackTextureSprite;

bool FloorCeiling::useFallbackTexture = false;

void FloorCeiling::init()
{
	useFallbackTexture = !shader.isAvailable();

	// These functions return false if there's something wrong
	if (!(shader.isAvailable() && shader.loadFromFile(SHADER_FILENAME, Shader::Fragment))) {
		useFallbackTexture = true;
	}
	
	if (useFallbackTexture) {
		fallbackTexture = TextureCache::load(FALLBACK_TEXTURE_FILENAME);
		fallbackTextureSprite = Sprite(fallbackTexture);

		// For some reason this gets drawn upside down by default, so I have to reverse the Y axis
		fallbackTextureSprite.setPosition(0, Graphics::BUFFER_HEIGHT);
		fallbackTextureSprite.setScale(Graphics::BUFFER_WIDTH / (float)fallbackTexture.getSize().x, -Graphics::BUFFER_HEIGHT / (float)fallbackTexture.getSize().y);
		fallbackTextureSprite.setColor(Color::White);
	}
	else {
		// The shader requires some data before it can be used
		shader.setUniform("fov_h", Camera::FOV_H);
		shader.setUniform("fov_v", Camera::FOV_V);

		shader.setUniform("fog_distance", Camera::FOG_DISTANCE);

		shader.setUniform("floor_texture", *TextureCache::get("floor.png"));
		shader.setUniform("ceiling_texture", *TextureCache::get("ceiling.png"));
	}
}

void FloorCeiling::update()
{
	if (useFallbackTexture) {
		return;
	}

	shader.setUniform("camera_pos", Camera::position);
	shader.setUniform("camera_rot", Camera::rotation);
}

void FloorCeiling::render()
{
	if (useFallbackTexture) {
		Graphics::buffer->draw(fallbackTextureSprite);
	}
	else {
		// For some reason, drawing a texture with a shader draws it upside down
		Graphics::drawTexture(0, Graphics::BUFFER_HEIGHT, Graphics::BUFFER_WIDTH, -Graphics::BUFFER_HEIGHT, &shader);
	}
}
