#include "title.h"
#include "../Play/camera.h"
#include "../Play/texture.h"

Texture *background;
extern bool keys[256];

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
	static bool presSpace = false;

	if (keys[' '] && presSpace == false)
	{
		onChangeScene = true;
	}
	presSpace = keys[0x0d];

	//�}�X�N��Z�����邽�߃A���t�@�l�𑝂₷
	if (onChangeScene)
	{
		maskAlpha += 1.f / 60;
	}

}

/////////////////////////////////
//�`��
/////////////////////////////////
void Title::draw()
{
	//RGB������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�J�������ʐݒ�
	camera->HUD();

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

	glEnable(GL_BLEND);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0, 0, 0, maskAlpha);
	glBegin(GL_QUADS);
	{
		glVertex3d(0, 0, 0);
		glVertex3d(5000, 0, 0);
		glVertex3d(5000, 5000, 0);
		glVertex3d(0, 5000, 0);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

/////////////////////////////////
//�폜
/////////////////////////////////
void Title::pDelete()
{
	delete background;
}

////////////////////////////////
//�V�[���ύX
////////////////////////////////
bool Title::changeScene()
{



	if (maskAlpha >= 0.8)
	{
		return true;
	}

	return false;
}

