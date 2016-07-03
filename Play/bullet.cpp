#include "bullet.h"
#include "field.h"
#include "pole.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet* > playerBullet;

////////////////////////////////////////
//更新
////////////////////////////////////////
void Bullet::update()
{

	pos += speed;	//移動
	//onCount++;
	
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

/////////////////////////////////////////
//円柱とのあたり判定
/////////////////////////////////////////
bool Bullet::hitPole()
{
	//円柱の数だけ確認する
	for (int i = 0; i < pole.size(); i++)
	{
		//弾と円柱の距離
		const float bulletToPole =
			(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
			+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);
		
		//円柱と当たった場合
		if (bulletToPole <= 2)
		{			
			pole[i]->HP -= damageSize;
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
	if (pos.x > field->center.x + 100 || pos.x < field->center.x - 100 ||
		pos.z > field->center.z + 100 || pos.z < field->center.z - 100)
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
	if (
		hitPole() ||				//円柱にあたっているか
		field->hitBullet(pos) ||	//フィールドにあたっているか
		outField())					//フィールド外にあるか
	{
		onExistFlag = false;	//もう存在しない
	}
}








