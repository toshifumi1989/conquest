#include "result.h"
#include "../Play/camera.h"
#include "../Play/pole.h"
#include "../glut.h"

extern bool keys[256];

//////////////////////////
//初期設定
//////////////////////////
void Result::init()
{


}


//////////////////////////
//更新
//////////////////////////
void Result::update()
{

}

///////////////////////////
//描画
///////////////////////////
void Result::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->HUD();

	const char poleNum = 3;	//1列の3つの円柱がある

	for (int y = 0; y < poleNum; y++)
	{
		for (int x = 0; x < poleNum; x++)
		{
			glPushMatrix();
			{
				glTranslatef(x * camera->right / poleNum, y * camera->top / poleNum, 0);

				const char poleCount = x + (y * 3);
				if (pole[poleCount]->type == TYPE::BLUE)
					glColor3f(0, 0, 1);
				else
					glColor3f(1, 0, 0);

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

///////////////////////////
//削除
///////////////////////////
void Result::pDelete()
{
	delete camera;

	pole.clear();
}


///////////////////////////
//シーン変更
///////////////////////////
bool Result::changeScene()
{
	static bool presEnter = false;

	if (keys[0x0d] && presEnter == false)
	{
		return true;
	}

	return false;

	presEnter = keys[0x0d];
}

