#define _USE_MATH_DEFINES

#include "player.h"
#include <math.h>
#include "field.h"

Player *player;
extern bool keys[256];

void Player::update()
{
	speed *= 0.9;	//����
	pos += speed;	//�ړ�

	pos.y = field->intersect(pos);
}

void Player::move()
{
	//�ړ�
	const float adjustSpeed = 0.05f;	//�����̒����p�ϐ�
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

	//�����̕ύX
	const float adjustYaw = 1;	//��]���x
	if (keys['a'])
	{
		yaw += adjustYaw;
	}
	else if (keys['d'])
	{
		yaw -= adjustYaw;
	}

}

void Player::attack()
{

}

void Player::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		const float adjustBody = 0.5f;	//�̂̃T�C�Y�̂��߂̈ʒu����

		glColor3f(color.r,color.g,color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);

		const char divideNum = 20;	//������
		glutSolidSphere(size, divideNum, divideNum);

		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}