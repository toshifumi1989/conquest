#include "deadEffect.h"
#include "../glut.h"
std::list< DeadEffect* >deadEffect;


/////////////////////////////////
//�X�V
/////////////////////////////////
void DeadEffect::update()
{
	onCount--;		//�����J�E���g�����炷
	pos += speed;
	speed *= 0.95;
}

///////////////////////////////////
//�`��
///////////////////////////////////
void DeadEffect::draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glPushMatrix();
	{
		glColor4f(color.r, color.g, color.b, 0.5f);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, 30, 30);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

///////////////////////////////////
//���݂�onCount��n��
///////////////////////////////////
char DeadEffect::count()
{
	return onCount;
}



