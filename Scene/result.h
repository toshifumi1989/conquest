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
	void init();			//�����ݒ�
	void update();			//���t���[���X�V
	void draw();			//�`��
	void background();		//�`��w�i
	void word(int _textureID, glm::vec3 _translate);//�`�敶��
	void pDelete();			//�폜
	bool changeScene();		//�V�[���؂�ւ�
};


#endif