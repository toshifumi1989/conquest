#include "npc.h"
#include "field.h"
#include "player.h"
#include "pole.h"
#include "bullet.h"
#include "deadEffect.h"
#include "../glut.h"
#include "wavFile.h"

std::list< NPC* >enemy;
std::list< NPC* > supporter;

//////////////////////////
//更新
///////////////////////////
void NPC::update(std::list< NPC* >_NPC)
{

	//攻撃目標が制圧できた場合次の攻撃目標を探す
	if (pole[targetID]->type == type)
	{
		targetPos = searchTarget();
	}

	targetPos = enemyTarget(_NPC);

	//敵とターゲットの距離
	const auto toTarget =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);

	//攻撃可能距離
	const auto onAttack = 30;

	//ターゲットの方向に向く
	yaw = atan2(targetPos.x - pos.x, targetPos.z - pos.z) * 180 / M_PI;

	attackCount--;
	stepCount--;

	move(toTarget, onAttack);
	attack(toTarget, onAttack);

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
//移動
/////////////////////////////////////
void NPC::move(float _distance, unsigned int _onAttack)
{
	//攻撃可能距離にいる場合
	if (_distance < _onAttack * _onAttack)
	{
		//ステップ可能になったら行動する
		if (stepCount <= 0)
		{
			const float stepSpeed = 1.f;
			if (rand() % 2 == 0)
			{//左右に移動する(回避行動
				speed.x += cos(yaw * M_PI / 180) * stepSpeed;
				speed.z -= sin(yaw * M_PI / 180) * stepSpeed;
			}
			else
			{
				speed.x -= cos(yaw * M_PI / 180) * stepSpeed;
				speed.z += sin(yaw * M_PI / 180) * stepSpeed;
			}

			stepCount = 30 + rand() % 40;//回避間隔の初期化(30～69
		}
	}
	else//攻撃できる距離まで移動する
	{
		const auto adjustSpeed = 0.5f;

		glm::vec3 targetVec(targetPos.x - pos.x, 0, targetPos.z - pos.z);
		speed = glm::normalize(targetVec) * adjustSpeed;
	}

	speed *= 0.9f;	//減速
	pos += speed;	//位置の移動

	pos.y = field->intersect(pos);	//y座標位置

	playerCollision();
	poleCollision();
	NPCCollision(enemy);
	NPCCollision(supporter);

	lastPos = pos;
}

/////////////////////////////////////
//プレイヤーとの当たり判定
/////////////////////////////////////
void NPC::playerCollision()
{
	const auto distance =
		(pos.x - player->pos.x) * (pos.x - player->pos.x)
		+ (pos.z - player->pos.z) * (pos.z - player->pos.z);

	if (distance < 2)
	{
		const auto miniDot = 0.2f;					//内積の最小値
		const auto reflection = 0.2f;				//反発率

		glm::vec3 axisVec = player->pos - pos;		//衝突軸ベクトル
		glm::normalize(axisVec);					//衝突軸の正規化

		float dot = glm::dot(speed, axisVec);		//内積算出
		if (dot < miniDot)
		{//内積を一定より下になるのを防ぐ
			dot = miniDot;
		}

		//反発方向へのベクトル
		glm::vec3 outVec = reflection * dot * axisVec;

		speed -= outVec;
		pos += speed;
	}
}

/////////////////////////////////////
//円柱との当たり判定
/////////////////////////////////////
void NPC::poleCollision()
{
	for (int i = 0; i < pole.size(); i++)
	{
		const auto distance =
			(pos.x - pole[i]->pos.x) * (pos.x - pole[i]->pos.x)
			+ (pos.z - pole[i]->pos.z) * (pos.z - pole[i]->pos.z);

		if (distance < 5)
		{
			const auto miniDot = 0.2f;					//内積の最小値
			const auto reflection = 0.3f;				//反発率

			glm::vec3 axisVec = pole[i]->pos - pos;		//衝突軸ベクトル
			glm::normalize(axisVec);					//衝突軸の正規化

			float dot = glm::dot(speed, axisVec);		//内積算出
			if (dot < miniDot)
			{//内積を一定より下になるのを防ぐ
				dot = miniDot;
			}

			//反発方向へのベクトル
			glm::vec3 outVec = reflection * dot * axisVec;

			speed -= outVec;
			pos += speed;
		}
	}
}


//////////////////////////////////
//NPCとの衝突判定
//////////////////////////////////
void NPC::NPCCollision(std::list< NPC* > _NPC)
{
	std::list< NPC* >::iterator iter = _NPC.begin();
	while (iter != _NPC.end())
	{
		if (*iter == this)
		{
			iter++;
			continue;
		}
		const auto distance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (distance < 2)
		{
			const auto miniDot = 0.2f;					//内積の最小値
			const auto reflection = 0.2f;				//反発率

			glm::vec3 axisVec = (*iter)->pos - pos;		//衝突軸ベクトル
			glm::normalize(axisVec);					//衝突軸の正規化

			float dot = glm::dot(speed, axisVec);		//内積算出
			if (dot < miniDot)
			{//内積を一定より下になるのを防ぐ
				dot = miniDot;
			}

			//反発方向へのベクトル
			glm::vec3 outVec = reflection * dot * axisVec;

			speed -= outVec;
			pos += speed;
		}

		iter++;
	}
}


//////////////////////////////
//攻撃
//////////////////////////////
void NPC::attack(float _distance, unsigned int _onAttack)
{
	const auto damage = 150;			//ダメージ量
	glm::vec3 correct(0, 0.5f, 0);		//補正

	//0以下の時、攻撃ができる
	if (attackCount <= 0)
	{
		//攻撃可能距離の内に敵がいた場合攻撃する
		if (_distance < _onAttack * _onAttack)
		{

			const float bulletYaw = yaw + (rand() % 30 - 15);
			Bullet* subBullet = new Bullet(pos + correct, bulletYaw, type, damage);
			bullet.push_back(subBullet);

			attackCount = 20 + rand() % 40;//攻撃間隔の初期化(20～59
		}
	}
}

//////////////////////////////
//攻撃対象を定める
//////////////////////////////
glm::vec3 NPC::searchTarget()
{
	auto mostNearPoleDistance = 99999;			//一番近いポールの距離を保存するための変数
	auto poleID = 0;							//一番近いポールの番号を保存

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

///////////////////////////////////////
//敵が近くにいたときに攻撃対象にする
///////////////////////////////////////
glm::vec3 NPC::enemyTarget(std::list< NPC* > _npc)
{
	//この関数内でターゲットの変更があったか確認
	bool changeTarget = false;

	//円柱の距離
	auto mostNearDistance =
		(pos.x - targetPos.x) * (pos.x - targetPos.x)
		+ (pos.z - targetPos.z) * (pos.z - targetPos.z);


	//プレイヤーとの所属が違う場合
	//プレイヤーがターゲットより近かったらターゲットにする
	if (type != player->playerType())
	{
		const float toPlayerDistance =
			(pos.x - player->pos.x) * (pos.x - player->pos.x)
			+ (pos.z - player->pos.z) * (pos.z - player->pos.z);

		if (mostNearDistance > toPlayerDistance)
		{
			mostNearDistance = toPlayerDistance;
			targetPos = player->pos;
			changeTarget = true;
		}
	}

	//ターゲットより近かったらターゲットにする
	std::list< NPC* >::iterator iter = _npc.begin();
	while (iter != _npc.end())
	{
		const float toNPCDistance =
			(pos.x - (*iter)->pos.x) * (pos.x - (*iter)->pos.x)
			+ (pos.z - (*iter)->pos.z) * (pos.z - (*iter)->pos.z);

		if (mostNearDistance > toNPCDistance)
		{
			mostNearDistance = toNPCDistance;
			targetPos = (*iter)->pos;
			changeTarget = true;
		}

		iter++;
	}

	if (changeTarget)
	{//もしターゲットの変更があったときは
		return targetPos;
	}
	else
	{//ターゲット変更がなかったとき円柱のまま
		return pole[targetID]->pos;
	}


}


////////////////////////////
//生存の確認
////////////////////////////
bool NPC::isDead()
{
	if (HP <= 0)
	{
		//死亡エフェクト生成
		for (int i = 0; i < 20; i++)
		{
			DeadEffect* deadEffe = new DeadEffect(pos, color);
			deadEffect.push_back(deadEffe);

			sound->playMusic(SOUND::ISDEAD);
		}
		return true;
	}
	else
	{
		return false;
	}
}

