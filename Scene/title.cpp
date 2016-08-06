#include "title.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/wavFile.h"

Texture *titleBackground;
Texture *titleWord;
Texture *titleNmae;
extern bool keys[256];
extern bool prevkeys[256];


////////////////////////////////////
//�^�C�g����ʏ����ݒ�
////////////////////////////////////
void Title::init()
{
	//�J��������
	camera = new Camera();

	//BGM�Đ�
	sound->playMusic(SOUND::TITLE_BGM);

	//�w�i
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_BACKGROUND]);
	titleBackground = new Texture();
	titleBackground->read("title.bmp");

	//����
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_SPACE]);
	titleWord = new Texture();
	titleWord->read_alpha("titleWord.bmp");

	//�^�C�g����
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_NAME]);
	titleNmae = new Texture();
	titleNmae->read_alpha("titleName.bmp");

}

//////////////////////////////////
//���t���[���X�V
//////////////////////////////////
void Title::update()
{
	//�N���b�N����2�b�ȏ�ɂȂ������~����
	if (sound->timeMusic(SOUND::CLICK) > 2.0f)
	{
		sound->stopMusic(SOUND::CLICK);
	}

	//�G���^�[����������V�[���J�ڂ���
	if (keys[0x0d] && prevkeys[0x0d] == false)
	{
		onChangeScene = true;
		sound->playMusic(SOUND::CLICK);
	}

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
	backGround();

	//�}�X�N
	mask();

	//�^�C�g������
	word(TEXTURE_ID::TITLE_SPACE, glm::vec3(600, 00, 0));

	//�^�C�g����
	word(TEXTURE_ID::TITLE_NAME, glm::vec3(600, 2500, 0));


}


//////////////////////////////////////////////////
//�^�C�g���w�i
//////////////////////////////////////////////////
void Title::backGround()
{
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

	glDisable(GL_TEXTURE_2D);

}

////////////////////////////////////////////////
//����
////////////////////////////////////////////////
void Title::word(int _textureID, glm::vec3 _translate)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, textures[_textureID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0, 0, 0, 1);
	glPushMatrix();
	{
		glTranslatef(_translate.x, _translate.y, _translate.z);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 1);
			glVertex3d(0, 0, 0);
			glTexCoord2d(1, 1);
			glVertex3d(4000, 0, 0);
			glTexCoord2d(1, 0);
			glVertex3d(4000, 2000, 0);
			glTexCoord2d(0, 0);
			glVertex3d(0, 2000, 0);
		}
		glEnd();
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}



//////////////////////////////////////
//�}�X�N
//////////////////////////////////////
void Title::mask()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	delete titleBackground;
	delete titleWord;

	sound->stopMusic(SOUND::TITLE_BGM);
	sound->stopMusic(SOUND::CLICK);
}

////////////////////////////////
//�V�[���ύX
////////////////////////////////
bool Title::changeScene()
{
	if (maskAlpha >= 0.9)
	{
		return true;
	}

	return false;
}

