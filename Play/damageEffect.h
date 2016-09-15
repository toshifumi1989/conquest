#ifndef DAMAGEEFFECT_H
#define DAMAGEEFFECT_H

#include <list>
#include "../glm/glm.hpp"
#include "gameObject.h"

class DamageEffect :public GameObject
{
public:
	DamageEffect(glm::vec3 _pos) :
		onCount(20)
	{
		const float adjustSpeed = 0.05f;
		pos = _pos;
		speed = glm::vec3(
			(rand() % 20 - 10) * adjustSpeed,
			(rand() % 20 - 10) * adjustSpeed,
			(rand() % 20 - 10) * adjustSpeed);
		size = 0.1f;
	}
	~DamageEffect() {}

	void update();	//�X�V
	void draw();	//�`��
	char count();	//���݂�onCount��n��

private:
	char onCount;	//0�ɂȂ�Ə���
};

extern std::list< DamageEffect* > damageEffect;

#endif