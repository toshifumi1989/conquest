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
//更新
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
//移動
///////////////////////////////////
void Player::move()
{
	glm::vec3 subSpeed(0, 0, 0);				//入力で速さに追加するベクトルの置き場
	const auto adjustSpeed = 0.05f;			//速さの調整用変数
	const auto adjustYaw = 1.2f;			//回転速度

	//移動-----------------------------------
	//縦移動
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

	//横移動
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

	//エラーが起こるため回避
	//	const auto length = subSpeed.length();	
	//	if (length > 0)
	if (subSpeed.x > 0 || subSpeed.z > 0)
	{//複数のボタンを押したときに距離が一定になるように正規化	
		subSpeed = glm::normalize(subSpeed);
	}

	speed.x += subSpeed.x * adjustSpeed;
	speed.z += subSpeed.z * adjustSpeed;

	//向きの変更
	if (specialKey[GLUT_KEY_LEFT])
	{
		yaw += adjustYaw;
	}
	else if (specialKey[GLUT_KEY_RIGHT])
	{
		yaw -= adjustYaw;
	}

	//減速
	speed *= 0.9;

	//移動
	pos += speed;

	//y軸フィールドの高さ
	pos.y = field->intersect(pos);

	//フィールドの外に出たら前のフレームの位置に戻る
	moveLimit();

	//位置の保存
	lastPos = pos;
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
void Player::attack()
{
	const auto adjustBody = size / 2;			//体のサイズのための位置調整
	const auto damage = 10 + chargeGauge * 5;	//ダメージ

	//チャージ
	if (maxChargeGauge > chargeGauge) chargeGauge++;

	//弾ターゲット更新
	bulletTarget();

	//↑を押すと弾を撃つ
	if (specialKey[GLUT_KEY_UP] && prevSpecialkey[GLUT_KEY_UP] == false)
	{
		Bullet *subBullet = new Bullet(pos + glm::vec3(0, adjustBody, 0), yaw, type, damage);
		bullet.push_back(subBullet);
		chargeGauge = 0;		//チャージ量の初期化
		sound->playMusic(SOUND::SHOOT);
	}

}

//////////////////////////////////////
//弾ターゲット
//////////////////////////////////////
void Player::bulletTarget()
{
	const auto adjustBody = size / 2;			//体のサイズのための位置調整
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
//死亡確認
////////////////////////////////////
bool Player::isDead()
{
	if (HP <= 0)
	{
		if (isDeathEffect == false)
		{
			//死亡エフェクト生成
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
//描画
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
//体の描画
/////////////////////////////////
void Player::bodyDraw()
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


/////////////////////////////////
//弾道描画
/////////////////////////////////
void Player::trajectoryDraw()
{
	const auto adjustBody = size / 2;	//プレイヤーの体のサイズのための位置調整
	const auto divideNum = 20;			//分割数
	const auto sphereSize = 0.2f;		//ターゲットの大きさ

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
//ヘッドアップディスプレイ
/////////////////////////////
void Player::HUD()
{
	bulletChargeGauge();	//チャージゲージ
	HPGauge();				//HPゲージ
	control();				//操作方法
	time->draw();			//残り時間
	map();
}

//////////////////////////////////
//弾のチャージゲージ
//////////////////////////////////
void Player::bulletChargeGauge()
{
	const auto gaugeHeight = 200;	//ゲージの高さ	
	const auto gaugeWidth = 700;	//ゲージの横幅
	const auto gaugeSize = ((float)chargeGauge / maxChargeGauge) * gaugeWidth;//現在のチャージ量

	//ゲージ表示
	glPushMatrix();
	{
		glTranslatef(4000, 700, 0);

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

	//文字表示
	glPushMatrix();
	{
		glColor3f(1, 1, 1);	//白
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
//HPゲージ
/////////////////////////////////////
void Player::HPGauge()
{
	const auto gaugeHeight = 200;	//ゲージの高さ	
	const auto gaugeWidth = 700;	//ゲージの横幅
	const float HPrate = (float)HP / maxHP;	// 現在のHP / 最大HP
	const auto HPsize = HPrate * gaugeWidth;//残りHPの割合

//ゲージ表示
	glPushMatrix();
	{
		glTranslatef(4000, 1200, 0);

		glBegin(GL_QUADS);
		{
			//maxゲージ(下地
			glColor3f(1, 1, 1);	//白色
			glVertex2f(0, 0);
			glVertex2f(gaugeWidth, 0);
			glVertex2f(gaugeWidth, gaugeHeight);
			glVertex2f(0, gaugeHeight);

			//現在のチャージ量
			if (HPrate > 0.5f) glColor3f(0, 1, 0);							//緑色
			else if (0.5f >= HPrate && HPrate > 0.3f) glColor3f(1, 1, 0);	//黄色
			else glColor3f(1, 0, 0);										//赤色
			glVertex2f(0, 0);
			glVertex2f(HPsize, 0);
			glVertex2f(HPsize, gaugeHeight);
			glVertex2f(0, gaugeHeight);
		}
		glEnd();
	}
	glPopMatrix();

	//文字表示
	glPushMatrix();
	{
		glColor3f(1, 1, 1);	//白
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
//操作方法
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
//ミニマップ
///////////////////////////////////
void Player::map()
{
	const auto height = 1500;	//ゲージの高さ	
	const auto width = 1500;	//ゲージの横幅

	glEnable(GL_BLEND);

	glPushMatrix();
	{
		glTranslatef(3300, 3300, 0);

		//下地の白-------------------------------
		glColor4f(1, 1, 1, 0.2f);
		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0);
			glVertex2f(width, 0);
			glVertex2f(width, height);
			glVertex2f(0, height);
		}
		glEnd();

		//円柱------------------------------------
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				const auto poleToPole = width / 4;	//円柱と円柱の幅

				glPushMatrix();
				{
					glTranslatef((3 - x) * poleToPole, (y + 1) * poleToPole, 0);

					const char poleCount = x + (y * 3);	//円柱番号

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
//プレイヤーの所属を判別する
//////////////////////////////
unsigned int Player::playerType()
{
	return type;
}



