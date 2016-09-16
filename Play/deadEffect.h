#ifndef DEADEFFECT_H
#define DEADEFFECT_H

#include <list>
#include "../glm/glm.hpp"
#include "textureEffect.h"

class DeadEffect : public GameObject 
{
public:
	DeadEffect(){}
	DeadEffect(glm::vec3 _pos){init(_pos);}
	~DeadEffect(){}

	void init(glm::vec3 _pos);
	void update();
	void draw();
	bool del();

private:
	std::list< TextureEffect > smoke;
	std::list< TextureEffect > fire;
};

extern std::list< DeadEffect* > deadEffect;

#endif