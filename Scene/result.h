#ifndef RESULT_H
#define RESULT_H

#include "scene.h"

class Result :public Scene
{
public:
	Result(){}
	~Result(){}

	void init();
	void update();
	void draw();
	void pDelete();
};


#endif