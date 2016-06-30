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
	Player(glm::vec3 _pos,float _size,float _yaw) :
		color(0.2f, 0.4f, 0.7f)
	{
		pos = _pos;
		size = _size;
		yaw = _yaw;
	}
	~Player(){}

	glm::vec3 color;		//キャラクターの色


	void update();					//更新
	void move();					//キャラクター移動
	void attack();					//攻撃
	void draw();					//描画

	
};

extern Player *player;

#endif