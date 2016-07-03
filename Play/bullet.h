#ifndef BULLET_H
#define BULLET_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <list>
#include "gameObject.h"

class Bullet :public GameObject {
public:
	Bullet(glm::vec3 _pos,float _yaw) :
		onExistFlag(true),
		onCount(0),
		damageSize(4)
	{
		const float bulletSpeed = 1.0f;

		pos = _pos + glm::vec3(0, 1, 0);
		yaw = _yaw;
		speed.x += sin(yaw * M_PI / 180) * bulletSpeed;
		speed.z += cos(yaw * M_PI / 180) * bulletSpeed;
	}
	~Bullet() {}

	void draw();
	void update();
	bool hitPole();
	bool outField();
	void exist();

	bool onExistFlag;

private:
	unsigned char onCount;
	unsigned char damageSize;


};

extern std::list< Bullet* > playerBullet;


#endif