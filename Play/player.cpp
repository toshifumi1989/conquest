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
//更新
///////////////////////////////////
void Player::update()
{
	speed *= 0.9;	//減速

	//移動
	pos += speed;

	//フィールドの外に出たら前のフレームの位置に戻る
	moveLimit();

	//y軸フィールドの高さ
	pos.y = field->intersect(pos);

	//チャージ
	charge();

	//位置の保存
	lastPos = pos;
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
//移動制限
//////////////////////////////////
void Player::moveLimit()
{
	//x軸(線の当たり判定
	if (field->outXaxisField(pos))
	{
		pos.x = lastPos.x;
	}

	//z軸(線の当たり判定
	if (field->outZaxisField(pos))
	{
		pos.z = lastPos.z;
	}

	poleCollision();
	NPCCollision(enemy);
	NPCCollision(supporter);

}


//////////////////////////////////
//円柱との衝突判定
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
			const auto miniDot = 0.2f;					//内積の最小値
			const auto reflection = 0.3f;				//反発率

			glm::vec3 axisVec = pole[i]->pos - pos;		//衝突軸ベクトル
			glm::normalize(axisVec);					//衝突軸の正規化

			float dot = glm::dot(speed, axisVec);		//内積算出
			if (dot < miniDot)
			{//内積を一定より下になるのを防ぐ
				dot = miniDot;
			}

			//反発方向へのベクトル
			glm::vec3 outVec = reflection * dot * axisVec;

			speed -= outVec;
			pos += speed;
		}
	}
}

//////////////////////////////////
//NPCとの衝突判定
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
			const auto miniDot = 0.2f;					//内積の最小値
			const auto reflection = 0.3f;				//反発率

			glm::vec3 axisVec = (*iter)->pos - pos;		//衝突軸ベクトル
			glm::normalize(axisVec);					//衝突軸の正規化

			float dot = glm::dot(speed, axisVec);		//内積算出
			if (dot < miniDot)
			{//内積を一定より下になるのを防ぐ
				dot = miniDot;
			}

			//反発方向へのベクトル
			glm::vec3 outVec = reflection * dot * axisVec;

			speed -= outVec;
			pos += speed;
		}

		iter++;
	}
}


//////////////////////////////////
//攻撃(キーボード
//////////////////////////////////
void Player::attackSpace()
{
	static bool presSpace = false;					//前のフレームでもスペースがtrueだったか確認
	const auto adjustBody = size / 2;			//体のサイズのための位置調整
	const auto damage = 50 + chargeGauge * 2;	//ダメージ


	if (keys[' '])
	{//スペースを押しているとチャージする
		isCharge = true;
	}

	if (keys[' '] == false && presSpace == true)
	{//スペースを離すと弾を撃つ
		Bullet *subBullet = new Bullet(pos + glm::vec3(0, adjustBody, 0), yaw, type, damage);
		bullet.push_back(subBullet);
		chargeGauge = 0;		//チャージ量の初期化
		isCharge = false;
	}

	presSpace = keys[' '];
}

/////////////////////////////////////
//マウス
/////////////////////////////////////
void Player::attackMouse(int _button, int _state)
{
	const auto adjustBody = size / 2;			//体のサイズのための発射位置調整
	const auto damage = 50 + chargeGauge * 2;	//ダメージ

	if (_button == GLUT_LEFT_BUTTON && _state == GLUT_DOWN)
	{//左クリックを押しているとチャージ
		isCharge = true;
	}

	if (_button == GLUT_LEFT_BUTTON && _state == GLUT_UP)
	{//左クリックを離すと攻撃
		Bullet *subBullet = new Bullet(pos + glm::vec3(0, adjustBody, 0), yaw, type, damage);
		bullet.push_back(subBullet);
		chargeGauge = 0;		//チャージ量の初期化
		isCharge = false;
	}
}

////////////////////////////////////
// isCharge:trueならチャージする
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
	shootMarker();			//ショットマーカー
	bulletChargeGauge();	//チャージゲージ
}

/////////////////////////////////
//ショットマーカー
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
//弾のチャージゲージ
//////////////////////////////////
void Player::bulletChargeGauge()
{
	const auto gaugeHeight = 200;
	const auto gaugeWidth = 700;
	const auto gaugeSize = ((float)chargeGauge / maxChargeGauge) * gaugeWidth;

	//ゲージ表示
	glPushMatrix();
	{
		glTranslatef(4000, 1000, 0);

		glBegin(GL_QUADS);
		{
			//maxゲージ(下地
			glColor3f(1, 1, 1);	//白色
			glVertex2f(0, 0);
			glVertex2f(gaugeWidth, 0);
			glVertex2f(gaugeWidth, gaugeHeight);
			glVertex2f(0, gaugeHeight);

			//現在のチャージ量
			glColor3f(0, 1, 0);	//緑色
			glVertex2f(0, 0);
			glVertex2f(gaugeSize, 0);
			glVertex2f(gaugeSize, gaugeHeight);
			glVertex2f(0, gaugeHeight);
		}
		glEnd();
	}
	glPopMatrix();

	//ChargeGauge文字表示
	glPushMatrix();
	{
		glColor3f(1, 1, 1);	//白
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
