#include "npc.h"
#include "field.h"
#include "pole.h"
#include "bullet.h"
#include "../glut.h"

std::list< NPC* >enemy;
std::list< NPC* > supporter;

//////////////////////////
//�X�V
///////////////////////////
void NPC::update()
{
	speed *= 0.9f;	//����
	pos += speed;	//�ʒu�̈ړ�

	pos.y = field->intersect(pos);	//y���W�ʒu

	attackCount--;
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
//�s���i�ړ��A�U��
/////////////////////////////////////
void NPC::action()
{
	//�U���ڕW�������ł����ꍇ���̍U���ڕW��T��
	if (pole[targetID]->type == type)
	{
		targetPos = searchTarget();
	}

	//�G�ƃ^�[�Q�b�g�̋���
	const auto enemyToTarget =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);

	//�U���\����
	const auto onAttack = 30;

	yaw = atan2(targetPos.x - pos.x, targetPos.z - pos.z) * 180 / M_PI;

	move(enemyToTarget, onAttack);
	attack(enemyToTarget, onAttack);

}


/////////////////////////////////////
//�ړ�
/////////////////////////////////////
void NPC::move(float _distance, unsigned int _onAttack)
{
	//�U���ł��鋗���܂ňړ�����
	if (!(_distance < _onAttack * _onAttack))
	{
		const auto adjustSpeed = 0.5f;

		glm::vec3 targetVec(targetPos.x - pos.x, 0, targetPos.z - pos.z);
		speed = glm::normalize(targetVec) * adjustSpeed;

	}

}

//////////////////////////////
//�U��
//////////////////////////////
void NPC::attack(float _distance, unsigned int _onAttack)
{
	const auto damage = 200;

	//0�ȉ��̎��A�U�����ł���
	if (attackCount <= 0)
	{
		//�U���\�����̓��ɓG�������ꍇ�U������
		if (_distance < _onAttack * _onAttack)
		{	
			Bullet* subBullet = new Bullet(pos, yaw, type, damage);
			enemyBullet.push_back(subBullet);

			attackCount = 60;//�U���Ԋu�̏�����
		}
	}
}

//////////////////////////////
//�U���Ώۂ��߂�
//////////////////////////////
glm::vec3 NPC::searchTarget()
{

	auto mostNearPoleDistance = 99999;			//��ԋ߂��|�[���̋�����ۑ����邽�߂̕ϐ�
	auto poleID = 999;							//��ԋ߂��|�[���̔ԍ���ۑ�

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


////////////////////////////
//�����̊m�F
////////////////////////////
bool NPC::onDead()
{
	if (HP <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
