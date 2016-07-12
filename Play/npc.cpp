#include "npc.h"
#include "field.h"
#include "pole.h"
#include "bullet.h"
#include "../glut.h"

std::list< NPC* >enemy;
std::list< NPC* > supporter;

//////////////////////////
//更新
///////////////////////////
void NPC::update()
{
	speed *= 0.9f;	//減速
	pos += speed;	//位置の移動

	pos.y = field->intersect(pos);	//y座標位置

	attackCount--;
}


///////////////////////////
//描画
///////////////////////////
void NPC::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);

		//キャラクター-------------------------------------
		const auto adjustBody = 0.5f;	//体のサイズのための位置調整

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y + adjustBody, pos.z);
		glRotatef(yaw, 0, 1, 0);

		const auto divideNum = 20;	//分割数
		glutSolidSphere(size, divideNum, divideNum);

		
		//HPバー------------------------------------------
		const auto height = 1;							//HPバーの高さ
		const auto showHP = HP / 100.f;					//現在のHP
		const auto showMaxHP = maxHP / 100.f;			//HPの最大
		const auto posHeight = 2;					//HPバーの高さ

		//ビルボード行列設定
		glm::mat4 view;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);

		glm::mat4 m = glm::inverse(view);
		m[3][0] = m[3][1] = m[3][2] = 0;
		glMultMatrixf((float*)&m);

		glTranslatef(-showMaxHP / 2.f, posHeight, 0);
		glBegin(GL_QUADS);
		{
			glColor3f(1, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(showHP, height, 0);
			glVertex3f(showHP, 0, 0);

			glColor3f(1, 1, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(showMaxHP, height, 0);
			glVertex3f(showMaxHP, 0, 0);
		}
		glEnd();
		
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}

/////////////////////////////////////
//行動（移動、攻撃
/////////////////////////////////////
void NPC::action()
{
	//攻撃目標が制圧できた場合次の攻撃目標を探す
	if (pole[targetID]->type == type)
	{
		targetPos = searchTarget();
	}

	//敵とターゲットの距離
	const auto enemyToTarget =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);

	//攻撃可能距離
	const auto onAttack = 30;

	yaw = atan2(targetPos.x - pos.x, targetPos.z - pos.z) * 180 / M_PI;

	move(enemyToTarget, onAttack);
	attack(enemyToTarget, onAttack);

}


/////////////////////////////////////
//移動
/////////////////////////////////////
void NPC::move(float _distance, unsigned int _onAttack)
{
	//攻撃できる距離まで移動する
	if (!(_distance < _onAttack * _onAttack))
	{
		const auto adjustSpeed = 0.5f;

		glm::vec3 targetVec(targetPos.x - pos.x, 0, targetPos.z - pos.z);
		speed = glm::normalize(targetVec) * adjustSpeed;

	}

}

//////////////////////////////
//攻撃
//////////////////////////////
void NPC::attack(float _distance, unsigned int _onAttack)
{
	const auto damage = 200;

	//0以下の時、攻撃ができる
	if (attackCount <= 0)
	{
		//攻撃可能距離の内に敵がいた場合攻撃する
		if (_distance < _onAttack * _onAttack)
		{	
			Bullet* subBullet = new Bullet(pos, yaw, type, damage);
			enemyBullet.push_back(subBullet);

			attackCount = 60;//攻撃間隔の初期化
		}
	}
}

//////////////////////////////
//攻撃対象を定める
//////////////////////////////
glm::vec3 NPC::searchTarget()
{

	auto mostNearPoleDistance = 99999;			//一番近いポールの距離を保存するための変数
	auto poleID = 999;							//一番近いポールの番号を保存

	//ポールの数を回す
	for (int i = 0; i < pole.size(); i++)
	{
		//ポールがエネミー側以外のものに限定
		if (!(pole[i]->type == type))
		{
			//ポールとエネミーの距離
			const float enemyToPole =
				(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
				+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);

			//今までのポールより近ければ保存
			if (enemyToPole < mostNearPoleDistance)
			{
				mostNearPoleDistance = enemyToPole;
				poleID = i;
			}
		}
	}


	//一番近いポールの位置を返す
	targetID = poleID;
	return pole[poleID]->pos;

}


////////////////////////////
//生存の確認
////////////////////////////
bool NPC::onDead()
{
	if (HP <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
