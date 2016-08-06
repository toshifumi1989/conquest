#ifndef DEADEFFECT_H
#define DEADEFFECT_H

#include <list>
#include "../glm/glm.hpp"
#include "gameObject.h"

class DeadEffect :public GameObject
{
public:
	DeadEffect(glm::vec3 _pos, glm::vec3 _color) :
		onCount(20)
	{
		const float adjustSpeed = 0.05f;
		pos = _pos;
		color = _color;
		speed = glm::vec3(
			(rand() % 20 - 10) * adjustSpeed,
			(rand() % 20 - 10) * adjustSpeed,
			(rand() % 20 - 10) * adjustSpeed);
		size = 0.1f;
	}
	~DeadEffect() {}

	void update();	//�X�V
	void draw();	//�`��
	char count();	//���݂�onCount��n��

private:
	char onCount;	//0�ɂȂ�Ə���
	glm::vec3 color;//�F
};

extern std::list< DeadEffect* >deadEffect;

#endif