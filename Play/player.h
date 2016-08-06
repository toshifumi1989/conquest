#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include "gameObject.h"
#include "npc.h"
#include "field.h"

/////////////////////////////////////
//プレイヤーキャラクターclass
/////////////////////////////////////
class Player : public GameObject
{
public:
	Player(glm::vec3 _pos,float _size,float _yaw,unsigned int _type):
		chargeGauge(0),
		maxChargeGauge(50),
		maxHP(500),
		HP(500),
		bulletTargetPoint(0,0,0)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		type = _type;
		if (_type == TYPE::BLUE)
		{//ブルー陣営
			color = glm::vec3(0.2f, 0.4f, 0.9f);
		}
		else if (_type == TYPE::RED)
		{//レッド陣営
			color = glm::vec3(0.9f, 0.2f, 0.1f);
		}
		else
		{//中立
			color = glm::vec3(1, 1, 1);
		}
	}
	~Player(){}

	void update();					//更新
	void draw();					//描画
	void HUD();						//ヘッドアップディスプレイ

	unsigned int playerType();		//プレイヤーの所属を渡す
	bool isDead();					//true 死亡

	int HP;							//耐久値

private:
	void move();					//キャラクター移動
	void moveLimit();				//移動制限場所の当たり判
	void poleCollision();			//円柱との衝突判定
	void NPCCollision(std::list< NPC* > _NPC);//NPCとの衝突判定
	void attack();					//攻撃
	void bulletTarget();			//弾のターゲット

	void bodyDraw();				//体の描画
	void trajectoryDraw();			//弾道描画

	void bulletChargeGauge();		//ショットゲージ
	void HPGauge();					//HPゲージ
	void control();					//操作方法
	void map();						//ミニマップ

	glm::vec3 color;				//キャラクターの色
	unsigned int type;				//所属陣営
	int chargeGauge;				//ショットチャージ量
	const int maxChargeGauge;		//ショットゲージの最大値
	const int maxHP;				//最大耐久値
	glm::vec3 bulletTargetPoint;	//弾の終着点
};

extern Player *player;
extern Texture *controller;

#endif