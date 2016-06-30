#include "wall.h"

std::vector< Wall* >wall;

//////////////////////////////////////////
//•`‰æ
/////////////////////////////////////////
void Wall::draw()
{
	const int wallTop = 120;
	const int wallButtom = -120;
	const int wallHeight = 120;

	glPushMatrix();
	{
		glColor3f(1, 1, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::WALL]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(yaw, 0, 1, 0);

		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 1);
			glVertex3d(wallTop, 0, 0);
			glTexCoord2d(1, 1);
			glVertex3d(wallButtom, 0, 0);
			glTexCoord2d(1, 0);
			glVertex3d(wallButtom, wallHeight, 0);
			glTexCoord2d(0, 0);
			glVertex3d(wallTop, wallHeight, 0);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();


}