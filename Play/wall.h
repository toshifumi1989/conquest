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

	void draw(const char _texterID);	//描画

private:
	glm::vec3 pos;		//位置
	float yaw;			//向き

};

extern std::vector< Wall* > wall;

#endif