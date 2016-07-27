#ifndef RESULT_H
#define RESULT_H

#include "scene.h"
#include "../glm/glm.hpp"

class Result :public Scene
{
public:
	Result(){}
	~Result(){}

	void init();
	void update();
	void draw();
	void background();
	void word(int _textureID, glm::vec3 _translate);
	void pDelete();
	bool changeScene();
};


#endif