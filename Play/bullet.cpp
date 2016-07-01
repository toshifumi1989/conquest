#include "bullet.h"
#include "field.h"
#include "pole.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet > playerBullet;

////////////////////////////////////////
//�X�V
////////////////////////////////////////
void Bullet::update()
{
	pos += speed;
	onCount++;
}

////////////////////////////////////////
//�`��
////////////////////////////////////////
void Bullet::draw() 
{
	const char divideNum = 20;
	glPushMatrix();
	{
		glColor3f(1, 1, 0);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, divideNum, divideNum);
	}
	glPopMatrix();
}

/////////////////////////////////////////
//�~���Ƃ̂����蔻��
/////////////////////////////////////////
void Bullet::hitPole()
{
	//�~���̐������m�F����
	for (int i = 0; i < pole.size(); i++)
	{

		//�e�Ɖ~���̋���
		const float bulletToPole =
			(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
			+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);
		
		//�~���Ɠ��������ꍇ
		if (bulletToPole <= 2)
		{			
			pole[i]->HP -= damageSize;
			onHitFlag = true;
		}

	}
}

//////////////////////////////////
//�t�B�[���h�Ƃ̂����蔻��
///////////////////////////////////
void Bullet::hitField()
{
	onHitFlag = field->hitBullet(pos);
}







