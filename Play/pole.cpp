#include "pole.h"
#include "../glut.h"
#include "player.h"

std::vector< Pole* >pole;

///////////////////////////////////////////
//ï`âÊ
////////////////////////////////////////////
void Pole::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		if (onPlayer)
		{
			color = player->color;
		}
		else
		{
			color = glm::vec3(1, 0, 0);
		}
		
		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x,pos.y,pos.z);
		glRotatef(-90, 1, 0, 0);

		GLUquadricObj *quad = gluNewQuadric();//quadric object ÇàÍÇ¬ê∂ê¨Ç∑ÇÈ 
		gluCylinder(quad, radius, radius, height, sides, 1);//â~íå


		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();

}