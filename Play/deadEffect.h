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

	void update();	//更新
	void draw();	//描画
	char count();	//現在のonCountを渡す

private:
	char onCount;	//0になると消滅
	glm::vec3 color;//色
};

extern std::list< DeadEffect* >deadEffect;

#endif