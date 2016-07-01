#include "bullet.h"
#include "field.h"
#include "pole.h"
#include "../glut.h"
#include "../glm/gtx/intersect.hpp"


std::list < Bullet > playerBullet;

////////////////////////////////////////
//更新
////////////////////////////////////////
void Bullet::update()
{
	pos += speed;
	onCount++;
}

////////////////////////////////////////
//描画
////////////////////////////////////////
void Bullet::draw() 
{
	const char divideNum = 20;
	glPushMatrix();
	{
		glColor3f(1, 1, 0);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, divideNum, divideNum);
	}
	glPopMatrix();
}

/////////////////////////////////////////
//円柱とのあたり判定
/////////////////////////////////////////
void Bullet::hitPole()
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
			onHitFlag = true;
		}

	}
}

//////////////////////////////////
//フィールドとのあたり判定
///////////////////////////////////
void Bullet::hitField()
{
	onHitFlag = field->hitBullet(pos);
}







