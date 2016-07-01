#include "pole.h"
#include "../glut.h"
#include "player.h"

std::vector< Pole* >pole;

//////////////////////////////////////////
//
//////////////////////////////////////////
void Pole::update()
{
	const float recovery = 0.1;
	if (onPlayer)
	{
		//プレイヤー側のときの自動回復
		//プレイヤー側のときの最大値はmaxHP
		if (HP < maxHP)	//HPがmaxになっていないなら
		{
			HP += recovery;
			if (HP >= maxHP)	//HPがmaxを超えたとき
			{
				HP = maxHP;		//HPをmaxにする
			}
		}
	}
	else
	{
		//エネミー側のときの自動回復
		//エネミー側のときの最大値は0
		if (HP > 0)	//HPが0より大きいとき
		{
			HP -= recovery;
			if (HP <= 0)		//HPが0を超えたとき
			{
				HP = 0;			//HPを0にする
			}
		}
	}
}

///////////////////////////////////////////
//描画
////////////////////////////////////////////
void Pole::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);


		//円柱---------------------------------------------------
		if (onPlayer)
		{
			color = player->color;
		}
		else
		{
			color = glm::vec3(1, 0, 0);
		}

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y, pos.z);


		glRotatef(-90, 1, 0, 0);//向きを立てる
		GLUquadricObj *quad = gluNewQuadric();//quadric object を一つ生成する 
		gluCylinder(quad, radius, radius, height, sides, 1);//円柱


		//HPバー---------------------------------------------------
		glRotatef(90, 1, 0, 0);//円柱の向きを戻す
		const float height = 1;				//HPバーの高さ
		const float playerPole = HP / 20.f;	//player側の部分
		const float maxShow = maxHP / 20.f;	//HPバーの横の長さ

		const float posHeight = 5.0f;
		glTranslatef(-maxShow / 2, posHeight, 0);
		glBegin(GL_QUADS);
		{
			//プレイヤー側のHP
			glColor3f(player->color.r, player->color.g, player->color.b);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(playerPole, height, 0);
			glVertex3f(playerPole, 0, 0);

			//エネミー側のHP
			glColor3f(1, 0, 0);
			glVertex3f(playerPole, 0, 0);
			glVertex3f(playerPole, 0, 0);
			glVertex3f(maxShow, 0, 0);
			glVertex3f(maxShow, 0, 0);
		}
		glEnd();


		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();

}