#define _USE_MATH_DEFINES

#include <math.h>
#include "player.h"
#include "bullet.h"
#include "field.h"

Player *player;
extern bool keys[256];

void Player::update()
{
	speed *= 0.9;	//減速
	pos += speed;	//移動

	pos.y = field->intersect(pos);
}

void Player::move()
{
	//移動
	const float adjustSpeed = 0.05f;	//速さの調整用変数
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

	//向きの変更
	const float adjustYaw = 1;	//回転速度
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
	static bool presSpace = 0;

	if (keys[' '] && presSpace == false)
	{
		Bullet *bullet = new Bullet(pos, yaw);
		playerBullet.push_back(bullet);
	}

	presSpace = keys[' '];
}

void Player::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		const float adjustBody = 0.5f;	//体のサイズのための位置調整

		glColor3f(color.r,color.g,color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);

		const char divideNum = 20;	//分割数
		glutSolidSphere(size, divideNum, divideNum);

		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}