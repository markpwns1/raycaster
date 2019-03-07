#include "stdafx.h"
#include "World.h"
#include <fstream>
#include "Camera.h"
#include "maths.h"
#include "graphics.h"

using namespace std;
using namespace maths;

const float World::WALL_HEIGHT = 2.0f;
const float World::CEILING_HEIGHT = 2.0f;

Vector2f World::spawnPoint;

vector<Wall*> World::walls;
int World::wallCount;

vector<Wall*> World::workedWalls;
int World::workedWallCount;

bool str_starts_with(string str, string startsWith) {
	return str.rfind(startsWith, 0) == 0;
}

void str_remove_occurences(string* str, char c)
{
	str->erase(remove(str->begin(), str->end(), c), str->end());
}

vector<string> str_split(string str, string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

Vector2f vec_from_str(string str)
{
	str_remove_occurences(&str, '>');
	str_remove_occurences(&str, '{');
	str_remove_occurences(&str, '}');
	str_remove_occurences(&str, '(');
	str_remove_occurences(&str, ')');

	vector<string> components = str_split(str, ",");
	Vector2f vec;
	vec.x = stof(components[0]);
	vec.y = stof(components[1]);
	return vec;
}

float dist_from_ln_seg(Vector2f v, Vector2f w, Vector2f p)
{
	float l2 = vec_length_squared(w - v);
	if (l2 == 0.0f) return vec_length(v - p);
	float t = limit(vec_dot(p - v, w - v) / l2, 0.0f, 1.0f);
	return vec_length((v + (w - v) * t) - p);
}

bool circle_intersect(Vector2f position, float radius, Wall wall) 
{
	Vector2f d = wall.pointB - wall.pointA;
	Vector2f f = wall.pointA - position;

	float a = vec_dot(d, d);//d.Dot(d);
	float b = 2.0f * vec_dot(f, d);//2 * f.Dot(d);
	float c = vec_dot(f, f) - radius * radius;

	return b * b - 4 * a * c > 0;
}


void World::cullWalls() {
	workedWalls = vector<Wall*>(walls);

	Vector2f fwd = Vector2f(1.0f, 0.0f);

	for (int i = 0; i < workedWalls.size();) {
		Vector2f pointA = vec_rotate(workedWalls[i]->pointA - Camera::position, -Camera::rotation);
		Vector2f pointB = vec_rotate(workedWalls[i]->pointB - Camera::position, -Camera::rotation);

		if (pointA.x < 0 && pointB.x < 0) {
			workedWalls.erase(workedWalls.begin() + i);
			continue;
		}

		if (pointA.x > Camera::VIEW_DISTANCE && pointB.x > Camera::VIEW_DISTANCE) {
			workedWalls.erase(workedWalls.begin() + i);
			continue;
		}

		if (vec_unsigned_angle_between(pointA, fwd) > Camera::FOV_H / 2.0f &&
			vec_unsigned_angle_between(pointB, fwd) > Camera::FOV_H / 2.0f &&
			((pointA.y > 0 && pointB.y > 0) || (pointA.y < 0 && pointB.y < 0))) {
			workedWalls.erase(workedWalls.begin() + i);
			continue;
		}
		
		++i;
	}

	//Vector2f position = ;

	std::sort(workedWalls.begin(), workedWalls.end(), [](Wall* a, Wall* b) {
		return dist_from_ln_seg(a->pointA, a->pointB, Camera::position) < dist_from_ln_seg(b->pointA, b->pointB, Camera::position);
	});

	workedWallCount = workedWalls.size();
	//cout << workedWalls.size() << endl;
}

void World::loadFromFile(string filename)
{
	walls = vector<Wall*>();

	ifstream file(filename);
	string line;

	const string currentTexture = Wall::DEFAULT_TEXTURE;

	while (getline(file, line))
	{
		if (line == "" || str_starts_with(line, "//")) {
			continue;
		}
		else if (str_starts_with(line, ">")) {
			str_remove_occurences(&line, '>');
			spawnPoint = vec_from_str(line);
		}
		else
		{
			auto segments = str_split(line, "/");
			auto p1 = vec_from_str(segments[0]);
			auto p2 = vec_from_str(segments[1]);
			walls.insert(walls.end(), new Wall(p1, p2));
		}
	}

	wallCount = walls.size();
}


