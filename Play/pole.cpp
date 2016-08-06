#include "pole.h"
#include "../glut.h"
#include "player.h"

std::vector< Pole* >pole;

//////////////////////////////////////////
//自動回復(ダメージを受けていると回復する
//////////////////////////////////////////
void Pole::recovery()
{
	//自陣のとき自動回復
	const auto recovery = 1;
	if (type == TYPE::BLUE)
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
	else if (type == TYPE::RED)
	{
		//エネミー側のときの自動回復
		//エネミー側のときの最大値は -maxHP
		if (HP > -maxHP)	//HPが-maxHPより大きいとき
		{
			HP -= recovery;
		}
		//HPが0を超えたときHPを-maxHPにする
		if (HP <= -maxHP)
		{
			HP = -maxHP;
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
		type = TYPE::BLUE;

	}


	if (HP <= -maxHP)
	{
		type = TYPE::RED;

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
		const glm::vec3 blueColor(0.2f, 0.4f, 0.7f);	//BLUE陣営の色
		const glm::vec3 redColor(0.7f, 0.2f, 0.1f);		//RED陣営の色

		//円柱---------------------------------------------------
		if (type == TYPE::BLUE)
		{
			color = blueColor;
		}
		else if (type == TYPE::RED)
		{
			color = redColor;
		}
		else
		{
			color = glm::vec3(1, 1, 1);
		}

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y, pos.z);

		glPushMatrix();
		glRotatef(-90, 1, 0, 0);							//向きを立てる
		GLUquadricObj *quad = gluNewQuadric();				//quadric object を一つ生成する 
		gluCylinder(quad, radius, radius, height, sides, 1);//円柱
		glPopMatrix();


		//HPバー---------------------------------------------------

		const auto height = 1;							//HPバーの高さ
		const auto playerPole = HP / 100.f;			//player側の部分
		const auto maxShow = maxHP / 100.f;			//HPバーの横の長さ
		const auto posHeight = 10.0f;					//HPバーの高さ

		//ビルボード行列設定--------
		glm::mat4 view;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);

		glm::mat4 m = glm::inverse(view);
		m[3][0] = m[3][1] = m[3][2] = 0;
		glMultMatrixf((float*)&m);
		//--------------------------

		glTranslatef(0, posHeight, 0);
		glBegin(GL_QUADS);
		{
			//プレイヤー側のHP
			glColor3f(blueColor.r, blueColor.g, blueColor.b);
			glVertex3f(-maxShow, 0, 0);
			glVertex3f(-maxShow, height, 0);
			glVertex3f(playerPole, height, 0);
			glVertex3f(playerPole, 0, 0);

			//エネミー側のHP
			glColor3f(redColor.r, redColor.g, redColor.b);
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



