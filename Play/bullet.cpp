#include "bullet.h"
#include "player.h"
#include "field.h"
#include "pole.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet* > playerBullet;
std::list < Bullet* > enemyBullet;

////////////////////////////////////////
//更新
////////////////////////////////////////
void Bullet::update()
{

	pos += speed;	//移動
	onCount--;
	
}

////////////////////////////////////////
//描画
////////////////////////////////////////
void Bullet::draw() 
{
	const float size = 0.1f;	//球の半径
	const char divideNum = 20;	//球の分割数

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glColor3f(1, 1, 0);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, divideNum, divideNum);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

////////////////////////////////////////
//キャラクターとの当たり判定
////////////////////////////////////////
bool Bullet::hitCharacter(std::list< NPC* > _character)
{
	//あたり判定の距離
	const float hitDistance = 2.f;

	std::list< NPC* >::iterator iter = _character.begin();
	while (iter != _character.end())
	{
		//弾とキャラクターの距離
		const float distance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (distance <= hitDistance)
		{
			//当たったとき
			(*iter)->HP -= damageSize;

			return true;
		}
	}
	//当たっていないときはfalse
	return false;
}

/////////////////////////////////////////
//円柱とのあたり判定
/////////////////////////////////////////
bool Bullet::hitPole()
{
	//あたり判定の距離
	const float hitDistance = 4.5f;

	//円柱の数だけ確認する
	for (int i = 0; i < pole.size(); i++)
	{
		//弾と円柱の距離
		const float bulletToPole =
			(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
			+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);
		
		//円柱と当たった場合
		if (bulletToPole <= hitDistance)
		{
			//同じ陣営のポールに当たっても反応しない
			if (!(type == pole[i]->type))
			{
				if (type == TYPE::BLUE)
				{
 					pole[i]->HP += damageSize;
				}
				else
				{
					pole[i]->HP -= damageSize;
				}
			}
			return true;
		}
	}

	//あたっていないときはfalse
	return false;
}


///////////////////////////////////
//フィールド外になったとき
///////////////////////////////////
bool Bullet::outField()
{
	if (field->outXaxisField(pos) || field->outZaxisField(pos))
	{
		//外に出たときはtrue
		return true;
	}

	//出ていないときはfalse
	return false;
}


//////////////////////////////////
//弾が存在しているか
/////////////////////////////////
void Bullet::exist()
{
	//残りカウントの生存確認
	if (onCount <= 0)
	{
		onExistFlag = false;
	}

	//当たり判定での存在確認
	if (
		hitPole() ||				//円柱にあたっているか
		field->hitBullet(pos) ||	//フィールドにあたっているか
		outField())					//フィールド外にあるか
	{
		onExistFlag = false;	//もう存在しない
	}
}








