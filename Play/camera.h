#ifndef CAMERA_H
#define CAMERA_H

#include "../glm/glm.hpp"

class Camera
{
public:
	Camera() :
		angle(60.0f),
		aspect(1000.f / 700.f),
		nearDraw(1),
		farDraw(500),

		left(0),
		right(5000),
		bottom(0),
		top(5000),

		pos(0, 0, 0),
		target(0, 0, 0),
		up(0, 1, 0)
	{}

	void setUp(glm::vec3 _pos, glm::vec3 _target);		//戦闘準備用
	void update();										//カメラの更新
	void draw();										//カメラの描画
	void HUD();											//２次元の描画
	
	float left;			//2Dの左側
	float right;		//2Dの右側
	float bottom;		//2Dの下側
	float top;			//2Dの上側

	glm::vec3 pos;		//カメラの位置

private:

	glm::vec3 lastPos;	//前のフレームでの位置
	glm::vec3 target;	//カメラの向き
	glm::vec3 lastTarget;//前のフレームのカメラの向き

	const float angle;		//表示角度
	const float aspect;		//アスペクト
	const float nearDraw;		//どれくらい近くまで描画するか
	const float farDraw;		//どれくらい遠くまで描画するか

	glm::vec3 up;		//カメラの上の向き

};

extern Camera *camera;

#endif