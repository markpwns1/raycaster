#pragma once

#include "stdafx.h"

using namespace sf;
using namespace std;

namespace maths {
	static float PI = 3.1415f;

	// DANGEROUS
	static inline int to_int(float f) {
		return (int)roundf(f);
	}

	static inline float limit(float value, float min, float max) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}

	static inline float rad(float deg)
	{
		return deg * (PI / 180.0f);
	}

	static inline float deg(float rad)
	{
		return rad * (180.0f / PI);
	}

	static inline Vector2f vec_from_rot(float rad)
	{
		return Vector2f(cosf(rad), sinf(rad));
	}

	static inline float vec_dot(const Vector2f a, const Vector2f b)
	{
		return a.x * b.x + a.y * b.y;
	}

	static inline float vec_cross(const Vector2f a, const Vector2f b)
	{
		return a.x * b.y - a.y * b.x;
	}

	static inline float vec_length(Vector2f vec) 
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y);
	}

	static inline float vec_length_squared(Vector2f vec) {
		return vec.x * vec.x + vec.y * vec.y;
	}

	static inline Vector2f vec_perp(Vector2f vec) 
	{
		return Vector2f(-vec.y, vec.x);
	}

	static inline Vector2f vec_normalize(Vector2f vec) {
		return vec / vec_length(vec);
	}

	static inline Vector2f vec_rotate(Vector2f vec, float rads) {
		return Vector2f(vec.x * cosf(rads) - vec.y * sinf(rads), vec.x * sinf(rads) + vec.y * cosf(rads));
	}

	static inline float vec_signed_angle(Vector2f a) {
		return atan2f(a.y, a.x);
	}

	static inline float vec_unsigned_angle(Vector2f a) {
		return atanf(a.y / a.x);
	}

	static inline float vec_unsigned_angle_between(Vector2f a, Vector2f b)
	{
		return acosf(vec_dot(a, b) / (vec_length(a) * vec_length(b)));
	}

	static inline float vec_signed_angle_between(Vector2f a, Vector2f b) {
		float d = vec_dot(a, b);
		float c = vec_cross(a, b);
		return atan2f(c, d);
	}

	static inline float vec_dist(Vector2f a, Vector2f b) {
		return vec_length(b - a);
	}

	static inline string vec_to_str(Vector2f vec) 
	{
		return "(" + to_string(vec.x) + ", " + to_string(vec.y) + ")";
	}
};