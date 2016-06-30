#ifndef TITLE_H
#define TITLE_H

#include "scene.h"

class Title :public Scene
{
public:
	Title(){}
	~Title(){}


	void init();
	void update();
	void draw();
	void pDelete();

};

#endif