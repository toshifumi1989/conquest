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

	//移動------------------------------------
	pos += speed;

	//フィールドの外に出たら前のフレームの位置に戻る
	if (field->outXaxisField(pos))
	{//x軸
		pos.x = lastPos.x;
	}

	if (field->outZaxisField(pos))
	{//z軸
		pos.z = lastPos.z;
	}

	//y軸フィールドの高さ
	pos.y = field->intersect(pos);

	lastPos = pos;	//位置の保存
}

void Player::move()
{

	const float adjustSpeed = 0.05f;		//速さの調整用変数
	const float adjustYaw = 1.2f;			//回転速度

	//移動-----------------------------------
	//縦移動
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

	//横移動
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


	//向きの変更
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
	static bool presSpace = 0;			//前のフレームでもスペースがtrueだったか確認
	const float adjustBody = size / 2;	//体のサイズのための位置調整

	if (keys[' '] && presSpace == false)
	{
		Bullet *bullet = new Bullet(pos + glm::vec3(0,adjustBody,0), yaw,true);
		playerBullet.push_back(bullet);
	}

	presSpace = keys[' '];
}

void Player::draw()
{
	const float adjustBody = size / 2;	//体のサイズのための位置調整
	const char divideNum = 20;			//分割数

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

