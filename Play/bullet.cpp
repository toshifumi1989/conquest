#include "bullet.h"
#include "player.h"
#include "field.h"
#include "pole.h"
#include "damageEffect.h"
#include "wavFile.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet* > bullet;


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
	
	//const auto size = 0.1f;		//���̔��a
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
//�v���C���[�Ƃ̓����蔻��
////////////////////////////////////////
bool Bullet::hitPlayer()
{
	//�����蔻��̋���
	const auto hitDistance = 1.f;

	//�e�ƃL�����N�^�[�̋���
	const auto distance =
		(pos.x - player->pos.x) * (pos.x - player->pos.x)
		+ (pos.z - player->pos.z) * (pos.z - player->pos.z);

	if (distance <= hitDistance)
	{//���������Ƃ�

		if (type != player->playerType())
		{//�������Ⴄ�Ȃ�_���[�W���󂯂�		
			player->HP -= damageSize;

			//�_���[�W�G�t�F�N�g����
			for (int i = 0; i < 20; i++)
			{
				DamageEffect* damageEffe = new DamageEffect(pos);
				damageEffect.push_back(damageEffe);

				sound->playMusic(SOUND::ISDEAD);
			}
		}
		return true;
	}
	return false;
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
		{//���������Ƃ�

			if (type != (*iter)->type)
			{//�������Ⴄ�Ȃ�_���[�W���󂯂�		
				(*iter)->HP -= damageSize;

				//�_���[�W�G�t�F�N�g����
				for (int i = 0; i < 20; i++)
				{
					DamageEffect* damageEffe = new DamageEffect(pos);
					damageEffect.push_back(damageEffe);

					sound->playMusic(SOUND::ISDEAD);
				}
			}
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
	const auto hitDistance = 5;

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
	if (onCount <= 0 ||				//�c��J�E���g�̐����m�F
		hitPlayer()||				//�v���C���[�Ɠ���������	
		hitCharacter(enemy) ||		//�G�l�~�[�Ɠ���������
		hitCharacter(supporter) ||	//�T�|�[�^�[�ɓ���������
		hitPole() ||				//�~���ɂ������Ă��邩
		field->hitBullet(pos) ||	//�t�B�[���h�ɂ������Ă��邩
		outField())					//�t�B�[���h�O�ɂ��邩
	{
		onExistFlag = false;	//�������݂��Ȃ�
	}
}




