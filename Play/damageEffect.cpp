#include "damageEffect.h"
#include "../glut.h"

std::list< DamageEffect* >damageEffect;


/////////////////////////////////
//更新
/////////////////////////////////
void DamageEffect::update()
{
	onCount--;		//生存カウントを減らす
	pos += speed;
	speed *= 0.95;
}

///////////////////////////////////
//描画
///////////////////////////////////
void DamageEffect::draw()
{
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glColor3f(1, 1, 0);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, 30, 30);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

///////////////////////////////////
//現在のonCountを渡す
///////////////////////////////////
char DamageEffect::count()
{
	return onCount;
}



