#define _USE_MATH_DEFINES

#include <math.h>
#include "player.h"
#include "bullet.h"
#include "field.h"

Player *player;
extern bool keys[256];

void Player::update()
{
	speed *= 0.9;	//����

	//�ړ�------------------------------------
	pos += speed;

	//�t�B�[���h�̊O�ɏo����O�̃t���[���̈ʒu�ɖ߂�
	if (field->outXaxisField(pos))
	{//x��
		pos.x = lastPos.x;
	}

	if (field->outZaxisField(pos))
	{//z��
		pos.z = lastPos.z;
	}

	//y���t�B�[���h�̍���
	pos.y = field->intersect(pos);

	lastPos = pos;	//�ʒu�̕ۑ�
}

void Player::move()
{

	const float adjustSpeed = 0.05f;		//�����̒����p�ϐ�
	const float adjustYaw = 1.2f;			//��]���x

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

void Player::attack()
{
	static bool presSpace = 0;			//�O�̃t���[���ł��X�y�[�X��true���������m�F
	const float adjustBody = size / 2;	//�̂̃T�C�Y�̂��߂̈ʒu����

	if (keys[' '] && presSpace == false)
	{
		Bullet *bullet = new Bullet(pos + glm::vec3(0,adjustBody,0), yaw,true);
		playerBullet.push_back(bullet);
	}

	presSpace = keys[' '];
}

void Player::draw()
{
	const float adjustBody = size / 2;	//�̂̃T�C�Y�̂��߂̈ʒu����
	const char divideNum = 20;			//������

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

