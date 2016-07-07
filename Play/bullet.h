#ifndef BULLET_H
#define BULLET_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <list>
#include "gameObject.h"

class Bullet :public GameObject {
public:
	Bullet(glm::vec3 _pos,float _yaw,unsigned int _type) :
		onExistFlag(true),
		onCount(0)
	{
		const float bulletSpeed = 2.0f;
		
		pos = _pos;
		yaw = _yaw;
		speed.x += sin(yaw * M_PI / 180) * bulletSpeed;
		speed.z += cos(yaw * M_PI / 180) * bulletSpeed;

		type = _type;
		if (_type == TYPE::BLUE)
		{
			damageSize = 100;
		}
		else if(_type == TYPE::RED)
		{
			damageSize = -100;
		}
		else
		{
			damageSize = 0;
		}
	}
	~Bullet() {}

	void draw();
	void update();
	bool hitPole();
	bool outField();
	void exist();

	bool onExistFlag;				//���݂��Ă��邩 true:�܂����݂��� false:���łɂȂ�

private:
	unsigned char onCount;			//�c��̐����J�E���g
	char damageSize;				//�_���[�W��
	unsigned int type;				//�����w�c


};

extern std::list< Bullet* > playerBullet;
extern std::list< Bullet* > enemyBullet;

#endif