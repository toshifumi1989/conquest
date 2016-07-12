#define _USE_MATH_DEFINES

#include <math.h>
#include "player.h"
#include "bullet.h"
#include "field.h"
#include "camera.h"
#include "texture.h"

Player *player;
Texture *mark;
extern bool keys[256];

///////////////////////////////////
//更新
///////////////////////////////////
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

///////////////////////////////////
//移動
///////////////////////////////////
void Player::move()
{

	const auto adjustSpeed = 0.05f;			//速さの調整用変数
	const auto adjustYaw = 1.2f;			//回転速度

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

//////////////////////////////////
//攻撃
//////////////////////////////////
void Player::attack()
{
	static bool presSpace = 0;			//前のフレームでもスペースがtrueだったか確認
	const auto adjustBody = size / 2;	//体のサイズのための位置調整
	auto damage = 100;					//ダメージ


	if (keys[' '] && presSpace == false)
	{
		Bullet *bullet = new Bullet(pos + glm::vec3(0,adjustBody,0), yaw,type, damage);
		playerBullet.push_back(bullet);
	}

	presSpace = keys[' '];
}

///////////////////////////////////
//描画
///////////////////////////////////
void Player::draw()
{
	const auto adjustBody = size / 2;	//体のサイズのための位置調整
	const auto divideNum = 20;			//分割数

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
//ヘッドアップディスプレイ
/////////////////////////////
void Player::HUD()
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
		glVertex3d((camera->right / 2) - 100, (camera->top / 2) + 200 , 0);
		glTexCoord2d(1, 1);
		glVertex3d((camera->right / 2) + 100, (camera->top / 2) + 200 , 0);
		glTexCoord2d(1, 0);
		glVertex3d((camera->right / 2) + 100, (camera->top / 2) + 450, 0);
		glTexCoord2d(0, 0);
		glVertex3d((camera->right / 2) - 100, (camera->top / 2) + 450, 0);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

}


