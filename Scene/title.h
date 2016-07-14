#ifndef TITLE_H
#define TITLE_H

#include "scene.h"

class Title :public Scene
{
public:
	Title():
		maskAlpha(0),
		onChangeScene(false)
	{}
	~Title(){}


	void init();
	void update();
	void draw();
	void pDelete();
	bool changeScene();

private:
	float maskAlpha;		//��ʂ����ɂ��邽�߂Ɏg���ϐ��i�}�X�N����邽��
	bool onChangeScene;		//�v���C���[���V�[���̑J�ڂ�I��������true
};

#endif