#include "npc.h"
#include "field.h"
#include "player.h"
#include "pole.h"
#include "bullet.h"
#include "deadEffect.h"
#include "../glut.h"
#include "wavFile.h"

std::list< NPC* >enemy;
std::list< NPC* > supporter;

//////////////////////////
//�X�V
///////////////////////////
void NPC::update(std::list< NPC* >_NPC)
{

	//�U���ڕW�������ł����ꍇ���̍U���ڕW��T��
	if (pole[targetID]->type == type)
	{
		targetPos = searchTarget();
	}

	targetPos = enemyTarget(_NPC);

	//�G�ƃ^�[�Q�b�g�̋���
	const auto toTarget =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);

	//�U���\����
	const auto onAttack = 30;

	//�^�[�Q�b�g�̕����Ɍ���
	yaw = atan2(targetPos.x - pos.x, targetPos.z - pos.z) * 180 / M_PI;

	attackCount--;
	stepCount--;

	move(toTarget, onAttack);
	attack(toTarget, onAttack);

}


///////////////////////////
//�`��
///////////////////////////
void NPC::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		//�L�����N�^�[-------------------------------------
		const auto adjustBody = 0.5f;	//�̂̃T�C�Y�̂��߂̈ʒu����

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);

		const auto divideNum = 20;	//������
		glutSolidSphere(size, divideNum, divideNum);


		//HP�o�[------------------------------------------
		const auto height = 1;							//HP�o�[�̍���
		const auto showHP = HP / 100.f;					//���݂�HP
		const auto showMaxHP = maxHP / 100.f;			//HP�̍ő�
		const auto posHeight = 2;					//HP�o�[�̍���

		//�r���{�[�h�s��ݒ�
		glm::mat4 view;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);

		glm::mat4 m = glm::inverse(view);
		m[3][0] = m[3][1] = m[3][2] = 0;
		glMultMatrixf((float*)&m);

		glTranslatef(-showMaxHP / 2.f, posHeight, 0);
		glBegin(GL_QUADS);
		{
			glColor3f(1, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(showHP, height, 0);
			glVertex3f(showHP, 0, 0);

			glColor3f(1, 1, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(showMaxHP, height, 0);
			glVertex3f(showMaxHP, 0, 0);
		}
		glEnd();

		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}

/////////////////////////////////////
//�ړ�
/////////////////////////////////////
void NPC::move(float _distance, unsigned int _onAttack)
{
	//�U���\�����ɂ���ꍇ
	if (_distance < _onAttack * _onAttack)
	{
		//�X�e�b�v�\�ɂȂ�����s������
		if (stepCount <= 0)
		{
			const float stepSpeed = 1.f;
			if (rand() % 2 == 0)
			{//���E�Ɉړ�����(����s��
				speed.x += cos(yaw * M_PI / 180) * stepSpeed;
				speed.z -= sin(yaw * M_PI / 180) * stepSpeed;
			}
			else
			{
				speed.x -= cos(yaw * M_PI / 180) * stepSpeed;
				speed.z += sin(yaw * M_PI / 180) * stepSpeed;
			}

			stepCount = 30 + rand() % 40;//����Ԋu�̏�����(30�`69
		}
	}
	else//�U���ł��鋗���܂ňړ�����
	{
		const auto adjustSpeed = 0.5f;

		glm::vec3 targetVec(targetPos.x - pos.x, 0, targetPos.z - pos.z);
		speed = glm::normalize(targetVec) * adjustSpeed;
	}

	speed *= 0.9f;	//����
	pos += speed;	//�ʒu�̈ړ�

	pos.y = field->intersect(pos);	//y���W�ʒu

	playerCollision();
	poleCollision();
	NPCCollision(enemy);
	NPCCollision(supporter);

	lastPos = pos;
}

/////////////////////////////////////
//�v���C���[�Ƃ̓����蔻��
/////////////////////////////////////
void NPC::playerCollision()
{
	const auto distance =
		(pos.x - player->pos.x) * (pos.x - player->pos.x)
		+ (pos.z - player->pos.z) * (pos.z - player->pos.z);

	if (distance < 2)
	{
		const auto miniDot = 0.2f;					//���ς̍ŏ��l
		const auto reflection = 0.2f;				//������

		glm::vec3 axisVec = player->pos - pos;		//�Փˎ��x�N�g��
		glm::normalize(axisVec);					//�Փˎ��̐��K��

		float dot = glm::dot(speed, axisVec);		//���ώZ�o
		if (dot < miniDot)
		{//���ς�����艺�ɂȂ�̂�h��
			dot = miniDot;
		}

		//���������ւ̃x�N�g��
		glm::vec3 outVec = reflection * dot * axisVec;

		speed -= outVec;
		pos += speed;
	}
}

/////////////////////////////////////
//�~���Ƃ̓����蔻��
/////////////////////////////////////
void NPC::poleCollision()
{
	for (int i = 0; i < pole.size(); i++)
	{
		const auto distance =
			(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
			+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);

		if (distance < 5)
		{
			const auto miniDot = 0.2f;					//���ς̍ŏ��l
			const auto reflection = 0.3f;				//������

			glm::vec3 axisVec = pole[i]->pos - pos;		//�Փˎ��x�N�g��
			glm::normalize(axisVec);					//�Փˎ��̐��K��

			float dot = glm::dot(speed, axisVec);		//���ώZ�o
			if (dot < miniDot)
			{//���ς�����艺�ɂȂ�̂�h��
				dot = miniDot;
			}

			//���������ւ̃x�N�g��
			glm::vec3 outVec = reflection * dot * axisVec;

			speed -= outVec;
			pos += speed;
		}
	}
}


//////////////////////////////////
//NPC�Ƃ̏Փ˔���
//////////////////////////////////
void NPC::NPCCollision(std::list< NPC* > _NPC)
{
	std::list< NPC* >::iterator iter = _NPC.begin();
	while (iter != _NPC.end())
	{
		if (*iter == this)
		{
			iter++;
			continue;
		}
		const auto distance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (distance < 2)
		{
			const auto miniDot = 0.2f;					//���ς̍ŏ��l
			const auto reflection = 0.2f;				//������

			glm::vec3 axisVec = (*iter)->pos - pos;		//�Փˎ��x�N�g��
			glm::normalize(axisVec);					//�Փˎ��̐��K��

			float dot = glm::dot(speed, axisVec);		//���ώZ�o
			if (dot < miniDot)
			{//���ς�����艺�ɂȂ�̂�h��
				dot = miniDot;
			}

			//���������ւ̃x�N�g��
			glm::vec3 outVec = reflection * dot * axisVec;

			speed -= outVec;
			pos += speed;
		}

		iter++;
	}
}


//////////////////////////////
//�U��
//////////////////////////////
void NPC::attack(float _distance, unsigned int _onAttack)
{
	const auto damage = 150;			//�_���[�W��
	glm::vec3 correct(0, 0.5f, 0);		//�␳

	//0�ȉ��̎��A�U�����ł���
	if (attackCount <= 0)
	{
		//�U���\�����̓��ɓG�������ꍇ�U������
		if (_distance < _onAttack * _onAttack)
		{

			const float bulletYaw = yaw + (rand() % 30 - 15);
			Bullet* subBullet = new Bullet(pos + correct, bulletYaw, type, damage);
			bullet.push_back(subBullet);

			attackCount = 20 + rand() % 40;//�U���Ԋu�̏�����(20�`59
		}
	}
}

//////////////////////////////
//�U���Ώۂ��߂�
//////////////////////////////
glm::vec3 NPC::searchTarget()
{
	auto mostNearPoleDistance = 99999;			//��ԋ߂��|�[���̋�����ۑ����邽�߂̕ϐ�
	auto poleID = 0;							//��ԋ߂��|�[���̔ԍ���ۑ�

	//�|�[���̐�����
	for (int i = 0; i < pole.size(); i++)
	{
		//�|�[�����G�l�~�[���ȊO�̂��̂Ɍ���
		if (!(pole[i]->type == type))
		{
			//�|�[���ƃG�l�~�[�̋���
			const float enemyToPole =
				(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
				+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);

			//���܂ł̃|�[�����߂���Εۑ�
			if (enemyToPole < mostNearPoleDistance)
			{
				mostNearPoleDistance = enemyToPole;
				poleID = i;
			}
		}
	}

	//��ԋ߂��|�[���̈ʒu��Ԃ�
	targetID = poleID;
	return pole[poleID]->pos;

}

///////////////////////////////////////
//�G���߂��ɂ����Ƃ��ɍU���Ώۂɂ���
///////////////////////////////////////
glm::vec3 NPC::enemyTarget(std::list< NPC* > _npc)
{
	//���̊֐����Ń^�[�Q�b�g�̕ύX�����������m�F
	bool changeTarget = false;

	//�~���̋���
	auto mostNearDistance =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);


	//�v���C���[�Ƃ̏������Ⴄ�ꍇ
	//�v���C���[���^�[�Q�b�g���߂�������^�[�Q�b�g�ɂ���
	if (type != player->playerType())
	{
		const float toPlayerDistance =
			(pos.x - player->pos.x) * (pos.x - player->pos.x)
			+ (pos.z - player->pos.z) * (pos.z - player->pos.z);

		if (mostNearDistance > toPlayerDistance)
		{
			mostNearDistance = toPlayerDistance;
			targetPos = player->pos;
			changeTarget = true;
		}
	}

	//�^�[�Q�b�g���߂�������^�[�Q�b�g�ɂ���
	std::list< NPC* >::iterator iter = _npc.begin();
	while (iter != _npc.end())
	{
		const float toNPCDistance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (mostNearDistance > toNPCDistance)
		{
			mostNearDistance = toNPCDistance;
			targetPos = (*iter)->pos;
			changeTarget = true;
		}

		iter++;
	}

	if (changeTarget)
	{//�����^�[�Q�b�g�̕ύX���������Ƃ���
		return targetPos;
	}
	else
	{//�^�[�Q�b�g�ύX���Ȃ������Ƃ��~���̂܂�
		return pole[targetID]->pos;
	}


}


////////////////////////////
//�����̊m�F
////////////////////////////
bool NPC::isDead()
{
	if (HP <= 0)
	{
		//���S�G�t�F�N�g����
		for (int i = 0; i < 20; i++)
		{
			DeadEffect* deadEffe = new DeadEffect(pos, color);
			deadEffect.push_back(deadEffe);

			sound->playMusic(SOUND::ISDEAD);
		}
		return true;
	}
	else
	{
		return false;
	}
}

