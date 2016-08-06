#ifndef BULLET_H
#define BULLET_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <list>
#include "npc.h"
#include "gameObject.h"

class Bullet :public GameObject {
public:
	Bullet(glm::vec3 _pos, float _yaw, unsigned int _type, int _damage) :
		onExistFlag(true)
	{
		const float bulletSpeed = 2.f;

		pos = _pos;
		yaw = _yaw;
		speed.x += sin(yaw * M_PI / 180) * bulletSpeed;
		speed.z += cos(yaw * M_PI / 180) * bulletSpeed;

		type = _type;
		damageSize = _damage;
		onCount = damageSize / 5;
		size = onCount * 0.01f;
	}
	~Bullet() {}

	void draw();					//描画
	void update();					//毎フレーム更新

	void exist();					//弾の生存確認

	bool onExistFlag;				//存在しているか true:まだ存在する false:すでにない

private:
	bool hitPlayer();				//プレイヤーに当たったか
	bool hitCharacter(std::list< NPC* > _character);//NPCに当たったか
	bool hitPole();					//円柱に当たったか
	bool outField();				//フィールド外に出たか

	char onCount;					//残りの生存カウント
	unsigned int damageSize;		//ダメージ量
	unsigned int type;				//所属陣営

};

extern std::list< Bullet* > bullet;


#endif