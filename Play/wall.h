#ifndef WALL_H
#define WALL_H

#include <vector>
#include "texture.h"
#include "../glm/glm.hpp"

class Wall :public Texture
{
public:
	Wall(glm::vec3 _pos,float _yaw)
	{	
		pos = _pos;
		yaw = _yaw;
	}
	~Wall(){}

	void draw();

private:
	glm::vec3 pos;
	float yaw;

};

extern std::vector< Wall* > wall;

#endif