#ifndef TITLE_H
#define TITLE_H

#include "scene.h"
#include "../glm/glm.hpp"

class Title :public Scene
{
public:
	Title():
		maskAlpha(0),
		onChangeScene(false)
	{}
	~Title(){}


	void init();			//�����ݒ�	
	void update();			//���t���[���X�V
	void draw();			//�`��
	void backGround();		//�w�i�`��
	void word(int _textureID, glm::vec3 _translate);//�`�敶��
	void mask();			//�}�X�N
	void pDelete();			//�폜	
	bool changeScene();		//�V�[���؂�ւ�

private:
	float maskAlpha;		//��ʂ����ɂ��邽�߂Ɏg���ϐ��i�}�X�N����邽��
	bool onChangeScene;		//�v���C���[���V�[���̑J�ڂ�I��������true
};

#endif