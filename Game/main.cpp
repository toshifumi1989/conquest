#include <stdlib.h>
#include <stdio.h>
#include "../glut.h"
#include "manager.h"
#include "../Play/texture.h"

bool keys[256] = {};

////////////////////////////////////////////////////
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
////////////////////////////////////////////////////
void init(void);
void display(void);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);





/////////////////////////////////////////////////
//�}�E�X
/////////////////////////////////////////////////
void mouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{

	}


}

//�}�E�X�̃{�^���������ꂽ���
void motion(int x, int y) 
{

	glutPostRedisplay();
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


}

void specialUp(int key, int x, int y)
{

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
	//initMusic();
}


///////////////////////////////////////
//�`��
////////////////////////////////////////
void display(void)
{

	Manager::getInstance()->update();

	glFlush();
}


////////////////////////////////////////////////////
//���C���֐�
////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitWindowSize(1000, 700);
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
