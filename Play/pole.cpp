#include "pole.h"
#include "../glut.h"
#include "player.h"
#include "enemy.h"

std::vector< Pole* >pole;

//////////////////////////////////////////
//自動回復
//////////////////////////////////////////
void Pole::recovery()
{
	//自陣のとき自動回復
	const int recovery = 1;
	if (type == TYPE::PLAYER)
	{
		//プレイヤー側のときの自動回復
		//プレイヤー側のときの最大値はmaxHP
		if (HP < maxHP)	//HPがmaxになっていないなら
		{
			HP += recovery;

		}
		//HPがmaxを超えたときHPをmaxにする
		if (HP >= maxHP)
		{
			HP = maxHP;
		}

	}
	else if (type == TYPE::ENEMY)
	{
		//エネミー側のときの自動回復
		//エネミー側のときの最大値は0
		if (HP > 0)	//HPが0より大きいとき
		{
			HP -= recovery;
		}
		//HPが0を超えたときHPを0にする
		if (HP <= 0)
		{
			HP = 0;
		}
	}

}


///////////////////////////////////////////
//占領  (タイプの切り替え
///////////////////////////////////////////
void Pole::occupation()
{
	if (HP >= maxHP)
	{
		type = TYPE::PLAYER;

	}


	if (HP <= 0)
	{
		type = TYPE::ENEMY;

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
		if (type == TYPE::PLAYER)
		{
			color = player->color;
		}
		else if (type == TYPE::ENEMY)
		{
			color = glm::vec3(0.7f, 0.2f, 0.1f);
		}
		else
		{
			color = glm::vec3(1, 1, 1);
		}

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y, pos.z);

		glPushMatrix();
		glRotatef(-90, 1, 0, 0);//向きを立てる
		GLUquadricObj *quad = gluNewQuadric();//quadric object を一つ生成する 
		gluCylinder(quad, radius, radius, height, sides, 1);//円柱
		glPopMatrix();
		
		//ビルボード行列設定--------
		glm::mat4 view;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);

		glm::mat4 m = glm::inverse(view);
		m[3][0] = m[3][1] = m[3][2] = 0;
		glMultMatrixf((float*)&m);
		//--------------------------
		//HPバー---------------------------------------------------

		const float height = 1;						//HPバーの高さ
		const float playerPole = HP / 50.f;			//player側の部分
		const float maxShow = maxHP / 50.f;			//HPバーの横の長さ
		const float posHeight = 10.0f;				//HPバーの高さ
		const glm::vec3 enemyColor(0.7f, 0.2f, 0.1f);//敵の色

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
			glColor3f(enemyColor.r, enemyColor.g, enemyColor.b);
			glVertex3f(playerPole, 0, 0);
			glVertex3f(playerPole, height, 0);
			glVertex3f(maxShow, height, 0);
			glVertex3f(maxShow, 0, 0);
		}
		glEnd();


		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();

}