#ifndef RESULT_H
#define RESULT_H

#include "scene.h"
#include "../glm/glm.hpp"

class Result :public Scene
{
public:
	Result(){}
	~Result(){}

private:
	void init();			//‰Šúİ’è
	void update();			//–ˆƒtƒŒ[ƒ€XV
	void draw();			//•`‰æ
	void background();		//•`‰æ”wŒi
	void word(int _textureID, glm::vec3 _translate);//•`‰æ•¶š
	void pDelete();			//íœ
	bool changeScene();		//ƒV[ƒ“Ø‚è‘Ö‚¦
};


#endif