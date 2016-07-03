#ifndef ENEMY_H
#define ENEMY_H

#include <list>
#include "gameObject.h"

class Enemy :public GameObject
{
public:
	Enemy(glm::vec3 _pos, float _size, float _yaw) :
		color(0.7f, 0.2f, 0.1f)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
	}
	~Enemy(){}

	void update();
	void draw();


private:
	glm::vec3 color;

};

extern std::list< Enemy* > enemy;

#endif