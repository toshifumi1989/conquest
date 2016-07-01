#define _USE_MATH_DEFINES

#include <stdlib.h>
#include "camera.h"
#include "player.h"
#include "../glut.h"

Camera *camera;

void Camera::setUp(glm::vec3 _pos, glm::vec3 _target)
{
	pos = _pos;
	target = _target;
}

void Camera::update()
{
	//カメラの移動
	const float cameraHeight = 2.0f;
	pos.x = player->pos.x - sin(player->yaw * M_PI / 180) * distance;
	pos.y = player->pos.y + cameraHeight;
	pos.z = player->pos.z - cos(player->yaw * M_PI / 180) * distance;


	//カメラターゲットの再設定
	target = glm::vec3(0, 1, 0) + player->pos;
}

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

void Camera::twoDimensionCamera()
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