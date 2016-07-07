#ifndef NPC_H
#define NPC_H

#include <list>
#include "gameObject.h"

class NPC :public GameObject
{
public:
	NPC(glm::vec3 _pos, float _size, float _yaw,unsigned char _type) :
		HP(500),
		attackCount(120)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		type = _type;
		if (_type == TYPE::BLUE)
		{
			color = glm::vec3 (0.2f, 0.4f, 0.7f);
		}
		else if(_type == TYPE::RED)
		{
			color = glm::vec3 (0.7f, 0.2f, 0.1f);
		}
		else
		{
			color = glm::vec3(1, 1, 1);
		}

		targetPos = searchTarget();
	}
	~NPC() {}

	void update();				//毎フレーム更新
	void draw();				//描画
	void action();				//行動（移動、攻撃
	int HP;						//耐久値

private:
	void move(float _distance, unsigned int _onAttack);		//移動
	void attack(float _distance, unsigned int _onAttack);	//攻撃
	unsigned int type;			//所属陣営
	glm::vec3 searchTarget();	//ターゲットの位置を確認
	glm::vec3 color;			//描画の色
	glm::vec3 targetPos;		//ターゲットの位置	
	int targetID;				//ターゲットの番号
	int attackCount;			//攻撃間隔のための変数
};

extern std::list< NPC* > enemy;
extern std::list< NPC* > supporter;

#endif