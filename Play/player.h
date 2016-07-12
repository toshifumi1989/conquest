#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include "gameObject.h"
#include "field.h"

/////////////////////////////////////
//プレイヤーキャラクターclass
/////////////////////////////////////
class Player : public GameObject
{
public:
	Player(glm::vec3 _pos,float _size,float _yaw,unsigned int _type) 
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
	void attack();					//攻撃
	void HUD();						//ヘッドアップディスプレイ

private:
	glm::vec3 color;				//キャラクターの色
	unsigned int type;				//所属陣営

};

extern Player *player;
extern Texture *mark;

#endif