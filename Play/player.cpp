#define _USE_MATH_DEFINES

#include "player.h"
#include <math.h>
#include "field.h"

Player *player;
extern bool keys[256];

void Player::update()
{
	speed *= 0.9;	//å∏ë¨
	pos += speed;	//à⁄ìÆ

	field->intersect(pos);
	pos.y = field->charcterHeight;
}

void Player::move()
{
	const float adjustSpeed = 0.05f;	//ë¨Ç≥ÇÃí≤êÆópïœêî

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

	if (keys['a'])
	{
		yaw += 1;
	}
	else if (keys['d'])
	{
		yaw -= 1;
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

		glColor3f(color.r,color.g,color.b);
		glTranslatef(pos.x, pos.y + 0.5f, pos.z);
		glRotatef(yaw, 0, 1, 0);

		glutSolidSphere(size, 20, 20);

		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}