#ifndef BULLET_H
#define BULLET_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <list>
#include "gameObject.h"

class Bullet :public GameObject {
public:
	Bullet(glm::vec3 _pos,float _yaw,bool _onPlayer) :
		onExistFlag(true),
		onCount(0)
	{
		const float bulletSpeed = 2.0f;
		
		pos = _pos;
		yaw = _yaw;
		speed.x += sin(yaw * M_PI / 180) * bulletSpeed;
		speed.z += cos(yaw * M_PI / 180) * bulletSpeed;

		onPlayer = _onPlayer;
		if (_onPlayer)
		{
			damageSize = 100;
		}
		else
		{
			damageSize = -100;
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
	bool onPlayer;					//true:�v���C���[�̒e false:�G�l�~�[�̒e


};

extern std::list< Bullet* > playerBullet;
extern std::list< Bullet* > enemyBullet;

#endif