#include "bullet.h"
#include "player.h"
#include "field.h"
#include "pole.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet* > playerBullet;
std::list < Bullet* > enemyBullet;

////////////////////////////////////////
//�X�V
////////////////////////////////////////
void Bullet::update()
{

	pos += speed;	//�ړ�
	onCount--;
	
}

////////////////////////////////////////
//�`��
////////////////////////////////////////
void Bullet::draw() 
{
	const auto size = 0.1f;	//���̔��a
	const auto divideNum = 20;	//���̕�����

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

////////////////////////////////////////
//�L�����N�^�[�Ƃ̓����蔻��
////////////////////////////////////////
bool Bullet::hitCharacter(std::list< NPC* > _character)
{
	//�����蔻��̋���
	const auto hitDistance = 1.f;

	std::list< NPC* >::iterator iter = _character.begin();
	while (iter != _character.end())
	{
		//�e�ƃL�����N�^�[�̋���
		const auto distance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (distance <= hitDistance)
		{
			//���������Ƃ�
			(*iter)->HP -= damageSize;

			return true;
		}

		iter++;
	}
	//�������Ă��Ȃ��Ƃ���false
	return false;
}

/////////////////////////////////////////
//�~���Ƃ̂����蔻��
/////////////////////////////////////////
bool Bullet::hitPole()
{
	//�����蔻��̋���
	const auto hitDistance = 4.5f;

	//�~���̐������m�F����
	for (int i = 0; i < pole.size(); i++)
	{
		//�e�Ɖ~���̋���
		const auto bulletToPole =
			(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
			+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);
		
		//�~���Ɠ��������ꍇ
		if (bulletToPole <= hitDistance)
		{
			//�����w�c�̃|�[���ɓ������Ă��������Ȃ�
			if (!(type == pole[i]->type))
			{
				if (type == TYPE::BLUE)
				{
 					pole[i]->HP += damageSize;
				}
				else
				{
					pole[i]->HP -= damageSize;
				}
			}
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
	if (field->outXaxisField(pos) || field->outZaxisField(pos))
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
	//�c��J�E���g�̐����m�F
	if (onCount <= 0)
	{
		onExistFlag = false;
	}

	//�����蔻��ł̑��݊m�F
	if (
		hitCharacter(enemy)||
		hitCharacter(supporter)||
		hitPole() ||				//�~���ɂ������Ă��邩
		field->hitBullet(pos) ||	//�t�B�[���h�ɂ������Ă��邩
		outField())					//�t�B�[���h�O�ɂ��邩
	{
		onExistFlag = false;	//�������݂��Ȃ�
	}
}








