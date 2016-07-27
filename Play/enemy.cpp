#include "enemy.h"
#include "field.h"
#include "pole.h"
#include "bullet.h"
#include "../glut.h"

std::list< Enemy* >enemy;

//////////////////////////
//�X�V
///////////////////////////
void Enemy::update()
{
	speed *= 0.9f;
	pos += speed;
	pos.y = field->intersect(pos);

	attackCount--;
}


///////////////////////////
//�`��
///////////////////////////
void Enemy::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		const float adjustBody = 0.5f;	//�̂̃T�C�Y�̂��߂̈ʒu����

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);

		const char divideNum = 20;	//������
		glutSolidSphere(size, divideNum, divideNum);

		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}

/////////////////////////////////////
//�s���i�ړ��A�U��
/////////////////////////////////////
void Enemy::action()
{
	//�U���ڕW�������ł����ꍇ���̍U���ڕW��T��
	if (pole[targetID]->type == TYPE::ENEMY)
	{
		targetPos = searchTarget();
	}

	//�G�ƃ^�[�Q�b�g�̋���
	const float enemyToTarget =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);

	const unsigned int onAttack = 50;	//�U���\����

	yaw = atan2(targetPos.x - pos.x, targetPos.z - pos.z) * 180 / M_PI;

	move(enemyToTarget, onAttack);
	attack(enemyToTarget, onAttack);

}


/////////////////////////////////////
//�ړ�
/////////////////////////////////////
void Enemy::move(float _distance, unsigned int _onAttack)
{
	//�U���ł��鋗���܂ňړ�����
	if (!(_distance < _onAttack * _onAttack))
	{
		float adjustSpeed = 0.5f;

		glm::vec3 targetVec(targetPos.x - pos.x, 0, targetPos.z - pos.z);
		speed = glm::normalize(targetVec) * adjustSpeed;

	}

}

//////////////////////////////
//�U��
//////////////////////////////
void Enemy::attack(float _distance, unsigned int _onAttack)
{
	printf("%d\n", attackCount);

	//0�ȉ��̎��A�U�����ł���
	if (attackCount <= 0)
	{
		//�U���\�����̓��ɓG�������ꍇ�U������
		if (_distance < _onAttack * _onAttack)
		{
			Bullet* subBullet = new Bullet(pos, yaw, false);
			enemyBullet.push_back(subBullet);

			attackCount = 60;//�U���Ԋu�̏�����
		}
	}
}

//////////////////////////////
//�U���Ώۂ��߂�
//////////////////////////////
glm::vec3 Enemy::searchTarget()
{

	float mostNearPoleDistance = 99999;	//��ԋ߂��|�[���̋�����ۑ����邽�߂̕ϐ�
	unsigned int poleID = 999;					//��ԋ߂��|�[���̔ԍ���ۑ�

	//�|�[���̐�����
	for (int i = 0; i < pole.size(); i++)
	{
		//�|�[�����G�l�~�[���ȊO�̂��̂Ɍ���
		if (!(pole[i]->type == TYPE::ENEMY))
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