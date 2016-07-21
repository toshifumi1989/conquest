#ifndef BULLET_H
#define BULLET_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <list>
#include "npc.h"
#include "gameObject.h"

class Bullet :public GameObject {
public:
	Bullet(glm::vec3 _pos, float _yaw, unsigned int _type, int _damage) :
		onExistFlag(true)
	{
		const float bulletSpeed = 2.f;

		pos = _pos;
		yaw = _yaw;
		speed.x += sin(yaw * M_PI / 180) * bulletSpeed;
		speed.z += cos(yaw * M_PI / 180) * bulletSpeed;

		type = _type;
		damageSize = _damage;
		onCount = damageSize / 5;
		size = onCount * 0.01f;
	}
	~Bullet() {}

	void draw();
	void update();
	bool hitPlayer();
	bool hitCharacter(std::list< NPC* > _character);
	bool hitPole();
	bool outField();
	void exist();

	bool onExistFlag;				//���݂��Ă��邩 true:�܂����݂��� false:���łɂȂ�

private:
	char onCount;					//�c��̐����J�E���g
	unsigned int damageSize;		//�_���[�W��
	unsigned int type;				//�����w�c

};

extern std::list< Bullet* > bullet;


#endif