#include <stdlib.h>
#include <stdio.h>
#include "../glut.h"
#include "../Play/player.h"
#include "manager.h"
#include "../Play/texture.h"
#include "../Play/wavFile.h"

bool keys[256] = {};
bool prevkeys[256] = {};
bool specialKey[256] = {};
bool prevSpecialkey[256] = {};

////////////////////////////////////////////////////
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
////////////////////////////////////////////////////
void init(void);
void display(void);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void special(int key, int x, int y);
void specialUp(int key, int x, int y);

/////////////////////////////////////////////////
//�}�E�X
/////////////////////////////////////////////////
void mouse(int button, int state, int x, int y)
{

}

//�}�E�X�̃{�^���������ꂽ���
void motion(int x, int y)
{

}
//�}�E�X�̃{�^����������Ă��Ȃ����
void passiveMotion(int x, int y)
{

}


//////////////////////////////////////////////////
//�L�[�{�[�h��ON�AOFF
//////////////////////////////////////////////////
void keyboard(unsigned char key, int x, int y)
{
	keys[key] = true;
}
void keyboardUp(unsigned char key, int x, int y)
{
	keys[key] = false;
}

void special(int key, int x, int y)
{
	specialKey[key] = true;
}
void specialUp(int key, int x, int y)
{
	specialKey[key] = false;
}

////////////////////////////////////////////////////
//�^�C�}�[�֐�
////////////////////////////////////////////////////
void timer(int value)
{
	glutPostRedisplay();

	// 1000�~���b�� 1 �b
	glutTimerFunc(1000 / 60, timer, 0);
}


////////////////////////////////////////////////////
//�����ݒ�
////////////////////////////////////////////////////
void init(void)
{
	glGenTextures(TEXTURE_ID::TEXTURE_MAX, textures);
	initMusic();
}


///////////////////////////////////////
//�`��
////////////////////////////////////////
void display(void)
{
	Manager::getInstance()->update();

	for (int i = 0; i < 256; i++)
	{
		prevkeys[i] = keys[i];
		prevSpecialkey[i] = specialKey[i];
	}

	printf("key[ %d  :  %d ]\n",keys[0x0d] ,prevkeys[0x0d]);
	glFlush();
}


////////////////////////////////////////////////////
//���C���֐�
////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitWindowSize(1200, 840);
	glutCreateWindow("game");

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);

	//�}�E�X����
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);

	//�L�[�{�[�h����
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	glutIgnoreKeyRepeat(GL_TRUE);

	init();
	glutMainLoop();

}
