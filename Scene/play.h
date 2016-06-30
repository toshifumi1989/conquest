#ifndef PLAY_H
#define PLAY_H

#include "scene.h"

class Play : public Scene
{
public:
	Play(){}
	~Play() {}


	void init();
	void update();
	void draw();
	void twoDimension();
	void pDelete();
};


#endif