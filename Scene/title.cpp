#include "title.h"
#include "../Play/camera.h"
#include "../Play/texture.h"

Texture *background;


////////////////////////////////////
//�^�C�g����ʏ����ݒ�
////////////////////////////////////
void Title::init()
{
	//�J��������
	camera = new Camera();

	//�w�i
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_BACKGROUND]);
	background = new Texture();
	background->read("title.bmp");


}

//////////////////////////////////
//���t���[���X�V
//////////////////////////////////
void Title::update()
{

}

/////////////////////////////////
//�`��
/////////////////////////////////
void Title::draw()
{
	//RGB������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�J�������ʐݒ�
	camera->twoDimensionCamera();

	//�^�C�g����ʔw�i
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_BACKGROUND]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1);
		glVertex3d(camera->left, camera->bottom, 0);
		glTexCoord2d(1, 1);
		glVertex3d(camera->right, camera->bottom, 0);
		glTexCoord2d(1, 0);
		glVertex3d(camera->right, camera->top, 0);
		glTexCoord2d(0, 0);
		glVertex3d(camera->left, camera->top, 0);
	}
	glEnd();

	//glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

/////////////////////////////////
//�폜
/////////////////////////////////
void Title::pDelete()
{

}