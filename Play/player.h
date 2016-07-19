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
		isCharge(false),
		chargeGauge(0),
		maxChargeGauge(100)
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
	void move();					//キャラクター移動
	void moveLimit();				//移動制限場所の当たり判
	void poleCollision();			//円柱との衝突判定
	void NPCCollision(std::list< NPC* > _NPC);//NPCとの衝突判定
	void attackSpace();				//攻撃(キーボード
	void attackMouse(int _button, int _state);//攻撃(マウス
	void charge();					//isCharge:true ならチャージする
	void HUD();						//ヘッドアップディスプレイ
	void shootMarker();				//ショットマーカー
	void bulletChargeGauge();		//ショットゲージ
	unsigned int playerTypa();		//プレイヤーの所属を判別する

private:
	glm::vec3 color;				//キャラクターの色
	unsigned int type;				//所属陣営
	bool isCharge;					//true:チャージ中
	int chargeGauge;				//ショットチャージ量
	const int maxChargeGauge;		//ショットゲージの最大値
};

extern Player *player;
extern Texture *mark;

#endif