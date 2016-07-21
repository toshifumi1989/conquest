#include "wall.h"
#include "field.h"
#include "camera.h"

std::vector< Wall* >wall;

//////////////////////////////////////////
//ï`âÊ
/////////////////////////////////////////
void Wall::draw(const char _texterID)
{

	const auto wallLeft = 120;		//ç∂í[
	const auto wallRight = -120;	//âEí[
	const auto wallHeight = 120;	//çÇÇ≥

	glPushMatrix();
	{
		glColor3f(1, 1, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, textures[_texterID]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(yaw, 0, 1, 0);

		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 1);
			glVertex3d(wallLeft, 0, 0);
			glTexCoord2d(1, 1);
			glVertex3d(wallRight, 0, 0);
			glTexCoord2d(1, 0);
			glVertex3d(wallRight, wallHeight, 0);
			glTexCoord2d(0, 0);
			glVertex3d(wallLeft, wallHeight, 0);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();


}