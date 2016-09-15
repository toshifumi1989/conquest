#include "damageEffect.h"
#include "../glut.h"

std::list< DamageEffect* >damageEffect;


/////////////////////////////////
//�X�V
/////////////////////////////////
void DamageEffect::update()
{
	onCount--;		//�����J�E���g�����炷
	pos += speed;
	speed *= 0.95;
}

///////////////////////////////////
//�`��
///////////////////////////////////
void DamageEffect::draw()
{
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glColor3f(1, 1, 0);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, 30, 30);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

///////////////////////////////////
//���݂�onCount��n��
///////////////////////////////////
char DamageEffect::count()
{
	return onCount;
}



