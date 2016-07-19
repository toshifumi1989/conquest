#ifndef ENEMY_H
#define ENEMY_H

#include <list>
#include "gameObject.h"

class Enemy :public GameObject
{
public:
	Enemy(glm::vec3 _pos, float _size, float _yaw) :
		color(0.7f, 0.2f, 0.1f),
		attackCount(120)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		targetPos = searchTarget();
	}
	~Enemy() {}

	void update();				//毎フレーム更新
	void draw();				//描画
	void action();				//行動（移動、攻撃
	void move(float _distance, unsigned int _onAttack);				//移動
	void attack(float _distance, unsigned int _onAttack);				//攻撃

	glm::vec3 searchTarget();	//ターゲットの位置を確認



private:
	const glm::vec3 color;		//描画の色
	glm::vec3 targetPos;		//ターゲットの位置	
	int targetID;				//ターゲットの番号
	int attackCount;	//攻撃間隔のための変数
};

extern std::list< Enemy* > enemy;

#endif