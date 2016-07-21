#include "time.h"
#include "../glut.h"
#include <stdio.h>

Time* time;

/////////////////////////////
//毎フレームの更新
/////////////////////////////
void Time::update()
{
	frame++;
	//frame += 100 / 60.0f;

	//----------------------------
	/*
	millisecond = frame % 60;

	if (rest % 60 == 0)
	{
		seconds++;
	}

	if (rest == 60)
	{
		seconds = 0;
		minutes++;
	}
	*/
	//-----------------------------
	const auto count = rest - frame;


	if (count % 60 == 0)
	{
		seconds--;
	}

	if (seconds < 0)
	{
		seconds = 59;
		minutes--;
	}

}

//////////////////////////////
//描画
//////////////////////////////
void Time::draw()
{

	glPushMatrix();
	{
		glTranslatef(2000, 4000, 0);
		glColor3f(0, 0, 0);
		char min[6];
		sprintf_s(min, "%.2d.", minutes);
		glScalef(3, 3, 0);
		glLineWidth(3);
		for (int i = 0; min[i] != 0; i++)
		{
			glutStrokeCharacter(
				GLUT_STROKE_ROMAN,			//void *font,int
				min[i]);						//character
		}

		char sec[6];
		sprintf_s(sec, "%.2d", seconds);
		for (int i = 0; sec[i] != 0; i++)
		{
			glutStrokeCharacter(
				GLUT_STROKE_ROMAN,			//void *font,int
				sec[i]);						//character
		}
	}
	glPopMatrix();
}

/////////////////////////////
//時間を渡す
/////////////////////////////
int Time::getMinutes()
{
	return minutes;
}


