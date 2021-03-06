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

	void update();	//更新
	void draw();	//描画
	char count();	//現在のonCountを渡す

private:
	char onCount;	//0になると消滅
};

extern std::list< DamageEffect* > damageEffect;

#endif