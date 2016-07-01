#ifndef BULLET_H
#define BULLET_H

#include"gameObject.h"
#include<list>


class Bullet :public GameObject {
public:
	Bullet(glm::vec3 _pos):
		onHitFlag(false),
		size(0.1f),
		onCount(0),
		damageSize(4)
	{
		pos = _pos;
	}
	~Bullet(){}

	
	bool onHitFlag;
	float size;
	unsigned char onCount;
	unsigned char damageSize;

	void draw();
	void update();
	void hitPole();
	void hitField();
};

extern std::list<Bullet> playerBullet;


#endif