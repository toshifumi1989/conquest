#ifndef DEADEFFECT_H
#define DEADEFFECT_H

#include <list>
#include "../glm/glm.hpp"
#include "effect.h"

class DeadEffect : public GameObject {
public:

	std::list< Effect* > smoke;
	std::list< Effect* > fire;

	void init();

};

extern std::list< DeadEffect* > deadEffect;

#endif