#include "result.h"
#include "../Play/camera.h"
#include "../Play/pole.h"
#include "../glut.h"
#include "../Play/texture.h"
#include "../Play/wavFile.h"

Texture *result;
Texture *resultWord;
extern bool keys[256];
extern bool prevkeys[256];

//////////////////////////
//‰Šúİ’è
//////////////////////////
void Result::init()
{
	//BGMÄ¶
	sound->playMusic(SOUND::RESULT_BGM);

	//‰~’Œ‚ÌŠ‘®ƒ`[ƒ€‚ğŠm”F
	int blueNum = 0;
	int redNum = 0;

	for (int i = 0; i < pole.size(); i++)
	{
		if (pole[i]->type == TYPE::BLUE)
		{
			blueNum++;
		}
		else if (pole[i]->type == TYPE::RED)
		{
			redNum++;
		}
	}

	//Ÿ”s
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::VICTORYorDEFEAT]);
	result = new Texture();
	if(blueNum > redNum) result->read_alpha("resultWin.bmp");
	else if (blueNum < redNum) result->read_alpha("resultLose.bmp");
	else result->read_alpha("resultDraw.bmp");

	//•¶š
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::RESULT_WORD]);
	resultWord = new Texture();
	resultWord->read_alpha("resultWord.bmp");

}


//////////////////////////
//XV
//////////////////////////
void Result::update()
{

}

///////////////////////////
//•`‰æ
///////////////////////////
void Result::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->HUD();

	//”wŒi
	background();

	//•¶š
	word(TEXTURE_ID::VICTORYorDEFEAT, glm::vec3(600,2500,0));

	word(TEXTURE_ID::RESULT_WORD, glm::vec3(600, 0, 0));

}

///////////////////////////////////
//”wŒi(‚Æ‚Á‚½‰~’Œ
///////////////////////////////////
void Result::background()
{
	const char poleNum = 3;	//1—ñ‚Ì3‚Â‚Ì‰~’Œ‚ª‚ ‚é

	//F‚ğ“h‚é
	for (int y = 0; y < poleNum; y++)
	{
		for (int x = 0; x < poleNum; x++)
		{
			glPushMatrix();
			{
				glTranslatef((2 - x) * camera->right / poleNum, y * camera->top / poleNum, 0);

				const char poleCount = x + (y * 3);
				if (pole[poleCount]->type == TYPE::BLUE)
					glColor3f(0, 0, 1);
				else if (pole[poleCount]->type == TYPE::RED)
					glColor3f(1, 0, 0);
				else
					glColor3f(1, 1, 1);

				glBegin(GL_QUADS);
				{
					glVertex2f(camera->left, camera->bottom);
					glVertex2f(camera->right / poleNum, camera->bottom);
					glVertex2f(camera->right / poleNum, camera->top / poleNum);
					glVertex2f(camera->left, camera->top / poleNum);
				}
				glEnd();
			}
			glPopMatrix();
		}
	}

	//‹«ŠEü‚ğ‘‚­
	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		{
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
			{
				glVertex3f((i + 1) * camera->right / poleNum, camera->bottom, 0);
				glVertex3f((i + 1) * camera->right / poleNum, camera->top, 0);

				glVertex3f(camera->left, (i + 1) * camera->top / poleNum, 0);
				glVertex3f(camera->right, (i + 1) * camera->top / poleNum, 0);
			}
			glEnd();
		}
		glPopMatrix();
	}

}

///////////////////////////////////
//•¶š
///////////////////////////////////
void Result::word(int _textureID, glm::vec3 _translate)
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

///////////////////////////
//íœ
///////////////////////////
void Result::pDelete()
{
	delete camera;

	pole.clear();
	sound->stopMusic(SOUND::RESULT_BGM);
}


///////////////////////////
//ƒV[ƒ“•ÏX
///////////////////////////
bool Result::changeScene()
{
	
	if (keys[0x0d] && prevkeys[0x0d] == false)
	{
		return true;
	}

}

