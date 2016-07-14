#define _USE_MATH_DEFINES

#include <math.h>
#include "player.h"
#include "bullet.h"
#include "field.h"
#include "pole.h"
#include "camera.h"
#include "texture.h"

Player *player;
Texture *mark;
extern bool keys[256];

///////////////////////////////////
//�X�V
///////////////////////////////////
void Player::update()
{
	speed *= 0.9;	//����

	//�ړ�
	pos += speed;

	//�t�B�[���h�̊O�ɏo����O�̃t���[���̈ʒu�ɖ߂�
	moveLimit();

	//y���t�B�[���h�̍���
	pos.y = field->intersect(pos);

	//�`���[�W
	charge();

	//�ʒu�̕ۑ�
	lastPos = pos;
}

///////////////////////////////////
//�ړ�
///////////////////////////////////
void Player::move()
{

	const auto adjustSpeed = 0.05f;			//�����̒����p�ϐ�
	const auto adjustYaw = 1.2f;			//��]���x

	//�ړ�-----------------------------------
	//�c�ړ�
	if (keys['w'])
	{
		speed.x += sin(yaw * M_PI / 180) * adjustSpeed;
		speed.z += cos(yaw * M_PI / 180) * adjustSpeed;
	}
	else if (keys['s'])
	{
		speed.x -= sin(yaw * M_PI / 180) * adjustSpeed;
		speed.z -= cos(yaw * M_PI / 180) * adjustSpeed;
	}


	//���ړ�
	if (keys['a'])
	{
		speed.x += cos(yaw * M_PI / 180) * adjustSpeed;
		speed.z -= sin(yaw * M_PI / 180) * adjustSpeed;
	}
	else if (keys['d'])
	{
		speed.x -= cos(yaw * M_PI / 180) * adjustSpeed;
		speed.z += sin(yaw * M_PI / 180) * adjustSpeed;
	}


	//�����̕ύX
	if (keys['j'])
	{
		yaw += adjustYaw;
	}
	else if (keys['l'])
	{
		yaw -= adjustYaw;
	}

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
void Player::attackSpace()
{
	static bool presSpace = false;					//�O�̃t���[���ł��X�y�[�X��true���������m�F
	const auto adjustBody = size / 2;			//�̂̃T�C�Y�̂��߂̈ʒu����
	const auto damage = 50 + chargeGauge * 2;	//�_���[�W


	if (keys[' '])
	{//�X�y�[�X�������Ă���ƃ`���[�W����
		isCharge = true;
	}

	if (keys[' '] == false && presSpace == true)
	{//�X�y�[�X�𗣂��ƒe������
		Bullet *subBullet = new Bullet(pos + glm::vec3(0, adjustBody, 0), yaw, type, damage);
		bullet.push_back(subBullet);
		chargeGauge = 0;		//�`���[�W�ʂ̏�����
		isCharge = false;
	}

	presSpace = keys[' '];
}

/////////////////////////////////////
//�}�E�X
/////////////////////////////////////
void Player::attackMouse(int _button, int _state)
{
	const auto adjustBody = size / 2;			//�̂̃T�C�Y�̂��߂̔��ˈʒu����
	const auto damage = 50 + chargeGauge * 2;	//�_���[�W

	if (_button == GLUT_LEFT_BUTTON && _state == GLUT_DOWN)
	{//���N���b�N�������Ă���ƃ`���[�W
		isCharge = true;
	}

	if (_button == GLUT_LEFT_BUTTON && _state == GLUT_UP)
	{//���N���b�N�𗣂��ƍU��
		Bullet *subBullet = new Bullet(pos + glm::vec3(0, adjustBody, 0), yaw, type, damage);
		bullet.push_back(subBullet);
		chargeGauge = 0;		//�`���[�W�ʂ̏�����
		isCharge = false;
	}
}

////////////////////////////////////
// isCharge:true�Ȃ�`���[�W����
////////////////////////////////////
void Player::charge()
{
	if (isCharge)
	{
		if (maxChargeGauge > chargeGauge)
			chargeGauge++;
	}
}




///////////////////////////////////
//�`��
///////////////////////////////////
void Player::draw()
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

/////////////////////////////
//�w�b�h�A�b�v�f�B�X�v���C
/////////////////////////////
void Player::HUD()
{
	shootMarker();			//�V���b�g�}�[�J�[
	bulletChargeGauge();	//�`���[�W�Q�[�W
}

/////////////////////////////////
//�V���b�g�}�[�J�[
////////////////////////////////
void Player::shootMarker()
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::MARK]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_BLEND);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1);
		glVertex3d((camera->right / 2) - 100, (camera->top / 2) + 200, 0);
		glTexCoord2d(1, 1);
		glVertex3d((camera->right / 2) + 100, (camera->top / 2) + 200, 0);
		glTexCoord2d(1, 0);
		glVertex3d((camera->right / 2) + 100, (camera->top / 2) + 450, 0);
		glTexCoord2d(0, 0);
		glVertex3d((camera->right / 2) - 100, (camera->top / 2) + 450, 0);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

//////////////////////////////////
//�e�̃`���[�W�Q�[�W
//////////////////////////////////
void Player::bulletChargeGauge()
{
	const auto gaugeHeight = 200;
	const auto gaugeWidth = 700;
	const auto gaugeSize = ((float)chargeGauge / maxChargeGauge) * gaugeWidth;

	//�Q�[�W�\��
	glPushMatrix();
	{
		glTranslatef(4000, 1000, 0);

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

	//ChargeGauge�����\��
	glPushMatrix();
	{
		glColor3f(1, 1, 1);	//��
		char word[] = "ChargeGauge";
		glTranslatef(3200, 1000, 0);
		glScalef(1, 2, 0);
		glLineWidth(2);
		for (int i = 0; word[i] != 0; i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, word[i]);
		}
	}
	glPopMatrix();
}
