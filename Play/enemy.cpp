#include "enemy.h"
#include "field.h"
#include "../glut.h"

std::list< Enemy* >enemy;

//////////////////////////
//更新
///////////////////////////
void Enemy::update()
{
	pos += speed;
	pos.y = field->intersect(pos);
}


///////////////////////////
//描画
///////////////////////////
void Enemy::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		const float adjustBody = 0.5f;	//体のサイズのための位置調整

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);

		const char divideNum = 20;	//分割数
		glutSolidSphere(size, divideNum, divideNum);

		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}
