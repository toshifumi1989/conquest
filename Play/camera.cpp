#define _USE_MATH_DEFINES

#include <stdlib.h>
#include "camera.h"
#include "player.h"
#include "field.h"
#include "../glut.h"

Camera *camera;


///////////////////////////////
//play画面準備
///////////////////////////////
void Camera::setUp(glm::vec3 _pos, glm::vec3 _target)
{
	pos = _pos;
	lastPos = _pos;
	target = _target;
	lastTarget = _target;
}

/////////////////////////////
//更新
/////////////////////////////
void Camera::update()
{

	//カメラの移動
	const float posHeight = 2.0f;	//カメラの高さ調整用変数
	const float distance = 7.0f;	//カメラの距離

	pos.x = player->pos.x - sin(player->yaw * M_PI / 180) * distance;
	pos.y = player->pos.y + posHeight;
	pos.z = player->pos.z - cos(player->yaw * M_PI / 180) * distance;

	//カメラターゲットの再設定
	const float targetHeight = 1.0f;//ターゲット位置
	target = glm::vec3(0, targetHeight, 0) + player->pos;

	//カメラ補完設定
	const float cameraSpeed = 0.2f;	//カメラ補完の速さ
	pos = lastPos + (pos - lastPos) * cameraSpeed;
	target = lastTarget + (target - lastTarget) * cameraSpeed;


	if (field->outXaxisField(pos))
	{
		pos.x = lastPos.x;
	}
	
	if (field->outZaxisField(pos))
	{
		pos.z = lastPos.z;
	}

	//位置の保存
	lastPos = pos;
	lastTarget = target;
}

//////////////////////////////
//3D描画設定
//////////////////////////////
void Camera::draw()
{

	glMatrixMode(GL_PROJECTION);	// 投影変換行列の設定
	glLoadIdentity();				// 変換行列の初期化

	//透視投影法の視体積gluPerspactive(th, w/h, near, far);
	gluPerspective(angle, aspect, nearDraw, farDraw);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		pos.x, pos.y, pos.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z);

}

/////////////////////////////
//ヘッドアップディスプレイ
/////////////////////////////
void Camera::HUD()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
		left, right,	//GLdouble left,  right,
		bottom, top,	//GLdouble bottom,  top, 
		10, -10);		//GLdouble zNear,  zFar

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1, 1, 1);

}
