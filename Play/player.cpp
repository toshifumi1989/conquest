#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include "player.h"
#include "bullet.h"
#include "field.h"
#include "pole.h"
#include "camera.h"
#include "texture.h"
#include "deadEffect.h"
#include "time.h"
#include "wavFile.h"

Player *player;
Texture *controller;
extern bool keys[256];
extern bool prevkeys[256];
extern bool specialKey[256];
extern bool prevSpecialkey[256];

///////////////////////////////////
//�X�V
///////////////////////////////////
void Player::update()
{
	if (isDead() == false)
	{
		move();
		attack();
	}
	else
	{
		if (HP < 0) HP = 0;
		isDeadTimer++;
	}
}

///////////////////////////////////
//�ړ�
///////////////////////////////////
void Player::move()
{
	glm::vec3 subSpeed(0, 0, 0);				//���͂ő����ɒǉ�����x�N�g���̒u����
	const auto adjustSpeed = 0.05f;			//�����̒����p�ϐ�
	const auto adjustYaw = 1.2f;			//��]���x

	//�ړ�-----------------------------------
	//�c�ړ�
	if (keys['w'])
	{
		subSpeed.x += sin(yaw * M_PI / 180);
		subSpeed.z += cos(yaw * M_PI / 180);
	}
	else if (keys['s'])
	{
		subSpeed.x -= sin(yaw * M_PI / 180);
		subSpeed.z -= cos(yaw * M_PI / 180);
	}

	//���ړ�
	if (keys['a'])
	{
		subSpeed.x += cos(yaw * M_PI / 180);
		subSpeed.z -= sin(yaw * M_PI / 180);
	}
	else if (keys['d'])
	{
		subSpeed.x -= cos(yaw * M_PI / 180);
		subSpeed.z += sin(yaw * M_PI / 180);
	}

	//�G���[���N���邽�߉��
	//	const auto length = subSpeed.length();	
	//	if (length > 0)
	if (subSpeed.x > 0 || subSpeed.z > 0)
	{//�����̃{�^�����������Ƃ��ɋ��������ɂȂ�悤�ɐ��K��	
		subSpeed = glm::normalize(subSpeed);
	}

	speed.x += subSpeed.x * adjustSpeed;
	speed.z += subSpeed.z * adjustSpeed;

	//�����̕ύX
	if (specialKey[GLUT_KEY_LEFT])
	{
		yaw += adjustYaw;
	}
	else if (specialKey[GLUT_KEY_RIGHT])
	{
		yaw -= adjustYaw;
	}

	//����
	speed *= 0.9;

	//�ړ�
	pos += speed;

	//y���t�B�[���h�̍���
	pos.y = field->intersect(pos);

	//�t�B�[���h�̊O�ɏo����O�̃t���[���̈ʒu�ɖ߂�
	moveLimit();

	//�ʒu�̕ۑ�
	lastPos = pos;
}

//////////////////////////////////
//�ړ�����
//////////////////////////////////
void Player::moveLimit()
{
	//x��(���̓����蔻��
	if (field->outXaxisField(pos))
	{
		pos.x = lastPos.x;
	}

	//z��(���̓����蔻��
	if (field->outZaxisField(pos))
	{
		pos.z = lastPos.z;
	}

	poleCollision();
	NPCCollision(enemy);
	NPCCollision(supporter);

}


//////////////////////////////////
//�~���Ƃ̏Փ˔���
//////////////////////////////////
void Player::poleCollision()
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
void Player::NPCCollision(std::list< NPC* > _NPC)
{
	std::list<NPC*>::iterator iter = _NPC.begin();
	while (iter != _NPC.end())
	{
		const auto distance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (distance < 2)
		{
			const auto miniDot = 0.2f;					//���ς̍ŏ��l
			const auto reflection = 0.3f;				//������

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


//////////////////////////////////
//�U��(�L�[�{�[�h
//////////////////////////////////
void Player::attack()
{
	const auto adjustBody = size / 2;			//�̂̃T�C�Y�̂��߂̈ʒu����
	const auto damage = 10 + chargeGauge * 5;	//�_���[�W

	//�`���[�W
	if (maxChargeGauge > chargeGauge) chargeGauge++;

	//�e�^�[�Q�b�g�X�V
	bulletTarget();

	//���������ƒe������
	if (specialKey[GLUT_KEY_UP] && prevSpecialkey[GLUT_KEY_UP] == false)
	{
		Bullet *subBullet = new Bullet(pos + glm::vec3(0, adjustBody, 0), yaw, type, damage);
		bullet.push_back(subBullet);
		chargeGauge = 0;		//�`���[�W�ʂ̏�����
		sound->playMusic(SOUND::SHOOT);
	}

}

//////////////////////////////////////
//�e�^�[�Q�b�g
//////////////////////////////////////
void Player::bulletTarget()
{
	const auto adjustBody = size / 2;			//�̂̃T�C�Y�̂��߂̈ʒu����
	glm::vec3 subTarget(0, adjustBody, 0);
	const float bulletSpeed = 2.f;
	const unsigned char count = (50 + chargeGauge * 3) / 5;

	for (int i = 0; i < count; i++)
	{
		subTarget.x += sin(yaw * M_PI / 180) * bulletSpeed;
		subTarget.z += cos(yaw * M_PI / 180) * bulletSpeed;
	}

	bulletTargetPoint = pos + subTarget;

}


/////////////////////////////////////
//���S�m�F
////////////////////////////////////
bool Player::isDead()
{
	if (HP <= 0)
	{
		if (isDeathEffect == false)
		{
			//���S�G�t�F�N�g����
			DeadEffect* deadEffe = new DeadEffect(pos);
			deadEffect.push_back(deadEffe);

			isDeathEffect = true;
		}
		return true;
	}
	else
	{
		return false;
	}

}

///////////////////////////////////
//�`��
///////////////////////////////////
void Player::draw()
{
	if (isDead() == false)
	{
		bodyDraw();
		trajectoryDraw();
	}
}

/////////////////////////////////
//�̂̕`��
/////////////////////////////////
void Player::bodyDraw()
{
	const auto adjustBody = size / 2;	//�̂̃T�C�Y�̂��߂̈ʒu����
	const auto divideNum = 20;			//������

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);
		glutSolidSphere(size, divideNum, divideNum);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

}


/////////////////////////////////
//�e���`��
/////////////////////////////////
void Player::trajectoryDraw()
{
	const auto adjustBody = size / 2;	//�v���C���[�̑̂̃T�C�Y�̂��߂̈ʒu����
	const auto divideNum = 20;			//������
	const auto sphereSize = 0.2f;		//�^�[�Q�b�g�̑傫��

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glPushMatrix();
	{
		glColor4f(1, 1, 0, 0.7f);
		glLineWidth(2);
		glBegin(GL_LINES);
		{
			glVertex3f(pos.x, pos.y + adjustBody, pos.z);
			glVertex3f(bulletTargetPoint.x, bulletTargetPoint.y, bulletTargetPoint.z);
		}
		glEnd();

	}
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

/////////////////////////////
//�w�b�h�A�b�v�f�B�X�v���C
/////////////////////////////
void Player::HUD()
{
	bulletChargeGauge();	//�`���[�W�Q�[�W
	HPGauge();				//HP�Q�[�W
	control();				//������@
	time->draw();			//�c�莞��
	map();
}

//////////////////////////////////
//�e�̃`���[�W�Q�[�W
//////////////////////////////////
void Player::bulletChargeGauge()
{
	const auto gaugeHeight = 200;	//�Q�[�W�̍���	
	const auto gaugeWidth = 700;	//�Q�[�W�̉���
	const auto gaugeSize = ((float)chargeGauge / maxChargeGauge) * gaugeWidth;//���݂̃`���[�W��

	//�Q�[�W�\��
	glPushMatrix();
	{
		glTranslatef(4000, 700, 0);

		glBegin(GL_QUADS);
		{
			//max�Q�[�W(���n
			glColor3f(1, 1, 1);	//���F
			glVertex2f(0, 0);
			glVertex2f(gaugeWidth, 0);
			glVertex2f(gaugeWidth, gaugeHeight);
			glVertex2f(0, gaugeHeight);

			//���݂̃`���[�W��
			glColor3f(0, 1, 0);	//�ΐF
			glVertex2f(0, 0);
			glVertex2f(gaugeSize, 0);
			glVertex2f(gaugeSize, gaugeHeight);
			glVertex2f(0, gaugeHeight);
		}
		glEnd();
	}
	glPopMatrix();

	//�����\��
	glPushMatrix();
	{
		glColor3f(1, 1, 1);	//��
		char word[] = "ChargeGauge";
		glTranslatef(3200, 700, 0);
		glScalef(1, 2, 0);
		glLineWidth(2);
		for (int i = 0; word[i] != 0; i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, word[i]);
		}
	}
	glPopMatrix();
}

/////////////////////////////////////
//HP�Q�[�W
/////////////////////////////////////
void Player::HPGauge()
{
	const auto gaugeHeight = 200;	//�Q�[�W�̍���	
	const auto gaugeWidth = 700;	//�Q�[�W�̉���
	const float HPrate = (float)HP / maxHP;	// ���݂�HP / �ő�HP
	const auto HPsize = HPrate * gaugeWidth;//�c��HP�̊���

//�Q�[�W�\��
	glPushMatrix();
	{
		glTranslatef(4000, 1200, 0);

		glBegin(GL_QUADS);
		{
			//max�Q�[�W(���n
			glColor3f(1, 1, 1);	//���F
			glVertex2f(0, 0);
			glVertex2f(gaugeWidth, 0);
			glVertex2f(gaugeWidth, gaugeHeight);
			glVertex2f(0, gaugeHeight);

			//���݂̃`���[�W��
			if (HPrate > 0.5f) glColor3f(0, 1, 0);							//�ΐF
			else if (0.5f >= HPrate && HPrate > 0.3f) glColor3f(1, 1, 0);	//���F
			else glColor3f(1, 0, 0);										//�ԐF
			glVertex2f(0, 0);
			glVertex2f(HPsize, 0);
			glVertex2f(HPsize, gaugeHeight);
			glVertex2f(0, gaugeHeight);
		}
		glEnd();
	}
	glPopMatrix();

	//�����\��
	glPushMatrix();
	{
		glColor3f(1, 1, 1);	//��
		char word[] = "HP";
		glTranslatef(3500, 1200, 0);
		glScalef(1, 2, 0);
		glLineWidth(2);
		for (int i = 0; word[i] != 0; i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, word[i]);
		}
	}
	glPopMatrix();
}

//////////////////////////////////////
//������@
//////////////////////////////////////
void Player::control()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::CONTROLLER]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	{
		glTranslatef(500, 300, 0);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 1);
			glVertex3d(0, 0, 0);
			glTexCoord2d(1, 1);
			glVertex3d(1500, 0, 0);
			glTexCoord2d(1, 0);
			glVertex3d(1500, 1500, 0);
			glTexCoord2d(0, 0);
			glVertex3d(0, 1500, 0);
		}
		glEnd();
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

///////////////////////////////////
//�~�j�}�b�v
///////////////////////////////////
void Player::map()
{
	const auto height = 1500;	//�Q�[�W�̍���	
	const auto width = 1500;	//�Q�[�W�̉���

	glEnable(GL_BLEND);

	glPushMatrix();
	{
		glTranslatef(3300, 3300, 0);

		//���n�̔�-------------------------------
		glColor4f(1, 1, 1, 0.2f);
		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0);
			glVertex2f(width, 0);
			glVertex2f(width, height);
			glVertex2f(0, height);
		}
		glEnd();

		//�~��------------------------------------
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				const auto poleToPole = width / 4;	//�~���Ɖ~���̕�

				glPushMatrix();
				{
					glTranslatef((3 - x) * poleToPole, (y + 1) * poleToPole, 0);

					const char poleCount = x + (y * 3);	//�~���ԍ�

					if (pole[poleCount]->type == TYPE::BLUE)
						glColor3f(0, 0, 1);
					else if (pole[poleCount]->type == TYPE::RED)
						glColor3f(1, 0, 0);
					else
						glColor3f(1, 1, 1);

					glBegin(GL_QUADS);
					{
						glVertex2f(-100, -100);
						glVertex2f(100, -100);
						glVertex2f(100, 100);
						glVertex2f(-100, 100);
					}
					glEnd();
				}
				glPopMatrix();
			}
		}
	}
	glPopMatrix();

	glDisable(GL_BLEND);
}


//////////////////////////////
//�v���C���[�̏����𔻕ʂ���
//////////////////////////////
unsigned int Player::playerType()
{
	return type;
}



