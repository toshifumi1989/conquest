#include "deadEffect.h"
#include "../glut.h"

std::list< DeadEffect* >deadEffect;


void DeadEffect::init()
{
	std::list <TextureEffect*>::iterator smokeIter = smoke.begin();
	while (smokeIter != smoke.end())
	{
		(*smokeIter)->pos = pos;
		smokeIter++;
	}


	
}