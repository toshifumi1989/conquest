#include "bullet.h"
#include "field.h"
#include "pole.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet* > playerBullet;

////////////////////////////////////////
//�X�V
////////////////////////////////////////
void Bullet::update()
{

	pos += speed;	//�ړ�
	//onCount++;
	
}

////////////////////////////////////////
//�`��
////////////////////////////////////////
void Bullet::draw() 
{
	const float size = 0.1f;	//���̔��a
	const char divideNum = 20;	//���̕�����

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glColor3f(1, 1, 0);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, divideNum, divideNum);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

/////////////////////////////////////////
//�~���Ƃ̂����蔻��
/////////////////////////////////////////
bool Bullet::hitPole()
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
			return true;
		}
	}

	//�������Ă��Ȃ��Ƃ���false
	return false;
}



///////////////////////////////////
//�t�B�[���h�O�ɂȂ����Ƃ�
///////////////////////////////////
bool Bullet::outField()
{
	if (pos.x > field->center.x + 100 || pos.x < field->center.x - 100 ||
		pos.z > field->center.z + 100 || pos.z < field->center.z - 100)
	{
		//�O�ɏo���Ƃ���true
		return true;
	}

	//�o�Ă��Ȃ��Ƃ���false
	return false;
}

//////////////////////////////////
//�e�����݂��Ă��邩
/////////////////////////////////
void Bullet::exist()
{
	if (
		hitPole() ||				//�~���ɂ������Ă��邩
		field->hitBullet(pos) ||	//�t�B�[���h�ɂ������Ă��邩
		outField())					//�t�B�[���h�O�ɂ��邩
	{
		onExistFlag = false;	//�������݂��Ȃ�
	}
}








