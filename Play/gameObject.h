#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../glm/glm.hpp"

//所属陣営
enum TYPE
{
	NEUTRAL,	//中立
	BLUE,		//ブルーチーム
	RED,		//レッドチーム
};

////////////////////////////////////
//ゲームオブジェクトクラス
////////////////////////////////////
class GameObject 
{
public:
	GameObject() :
		pos(0, 0, 0),
		lastPos(0, 0, 0),
		speed(0, 0, 0),
		size(1),
		yaw(0)
	{}
	virtual ~GameObject() {}

	glm::vec3 pos;		//現在地
	glm::vec3 lastPos;	//１フレーム前の位置
	glm::vec3 speed;	//移動速さ
	float yaw;			//オブジェクトの向き

protected:
	float size;			//オブジェクトの大きさ
};

#endif