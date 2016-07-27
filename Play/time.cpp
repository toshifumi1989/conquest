#include "time.h"
#include "../glut.h"
#include <stdio.h>

Time* time;

/////////////////////////////
//–ˆƒtƒŒ[ƒ€‚ÌXV
/////////////////////////////
void Time::update()
{
	frame++;
	//frame += 100 / 60.0f;

	//----------------------------
	/*‰ÁZ‚³‚ê‚Ä‚¢‚­
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
	//-----------------------------
	*/
	//Œ¸Z‚³‚ê‚Ä‚¢‚­
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
//•`‰æ
//////////////////////////////
void Time::draw()
{

	glPushMatrix();
	{
		glTranslatef(500, 4000, 0);

		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0);
			glVertex2f(1200, 0);
			glVertex2f(1200, 500);
			glVertex2f(0, 500);
		}
		glEnd();

		glTranslatef(100, 100, 0);
		glColor3f(1, 1, 1);
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
//ŠÔ‚ğ“n‚·
/////////////////////////////
int Time::getMinutes()
{
	return minutes;
}


