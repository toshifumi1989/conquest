#include "deadEffect.h"
#include "../glut.h"
std::list< DeadEffect* >deadEffect;


/////////////////////////////////
//更新
/////////////////////////////////
void DeadEffect::update()
{
	onCount--;		//生存カウントを減らす
	pos += speed;
	speed *= 0.95;
}

///////////////////////////////////
//描画
///////////////////////////////////
void DeadEffect::draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glPushMatrix();
	{
		glColor4f(color.r, color.g, color.b, 0.5f);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(size, 30, 30);
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

///////////////////////////////////
//現在のonCountを渡す
///////////////////////////////////
char DeadEffect::count()
{
	return onCount;
}



