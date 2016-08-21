#ifndef NPC_H
#define NPC_H

#include <list>
#include "gameObject.h"

class NPC :public GameObject
{
public:
	NPC(glm::vec3 _pos, float _size, float _yaw,unsigned char _type) :
		maxHP(500),
		HP(500),
		attackCount(120),
		stepCount(120),
		isDeadTimer(0),
		isDeathEffect(false)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		type = _type;
		if (_type == TYPE::BLUE)
		{//ブルー陣営
			color = glm::vec3 (0.2f, 0.4f, 0.9f);
		}
		else if(_type == TYPE::RED)
		{//レッド陣営
			color = glm::vec3 (0.9f, 0.2f, 0.1f);
		}
		else
		{//中立
			color = glm::vec3(1, 1, 1);
		}

		targetPos = searchTarget();
	}
	~NPC() {}

	void update(std::list< NPC* > _NPC);//毎フレーム更新
	void draw();				//描画
	bool isDead();				//生存確認 true:死亡 false:生存
	void playerCollision();		//プレイヤーとの当たり判定
	void poleCollision();		//円柱との衝突判定
	void NPCCollision(std::list< NPC* > _NPC);//NPCとの衝突判定

	int HP;							//耐久値
	unsigned int type;				//所属陣営
	unsigned int isDeadTimer;		//死んでからの時間

private:
	void move(float _distance, unsigned int _onAttack);		//移動
	void attack(float _distance, unsigned int _onAttack);	//攻撃
	
	glm::vec3 searchTarget();	//ターゲットの位置を確認
	glm::vec3 enemyTarget(std::list< NPC* >_npc);	//敵ターゲットの位置
	glm::vec3 color;			//描画の色
	glm::vec3 targetPos;		//ターゲットの位置	
	int targetID;				//ターゲットの番号
	int attackCount;			//攻撃間隔のための変数
	int stepCount;				//回避行動の間隔のための変数
	unsigned int maxHP;			//最大耐久値
	bool isDeathEffect;			//死亡エフェクトフラグ
};

extern std::list< NPC* > enemy;
extern std::list< NPC* > supporter;

#endif