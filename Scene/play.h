#ifndef PLAY_H
#define PLAY_H

#include "scene.h"

/////////////////////////////
//�Q�[���v���C�V�[��
/////////////////////////////
class Play : public Scene
{
public:
	Play(){}
	~Play() {}

	void init();		//�����ݒ�
	void update();		//�X�V
	void draw();		//�`��
	void HUD();			//�w�b�h�A�b�v�f�B�X�v���C
	void pDelete();		//�폜
	bool changeScene();	//�V�[���ύX�̏���

};


#endif