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
	//�J�����̈ړ�
	const float posHeight = 2.0f;	//�J�����̍��������p�ϐ�
	const float distance = 7.0f;	//�J�����̋���
	pos.x = player->pos.x - sin(player->yaw * M_PI / 180) * distance;
	pos.y = player->pos.y + posHeight;
	pos.z = player->pos.z - cos(player->yaw * M_PI / 180) * distance;

	//�J�����^�[�Q�b�g�̍Đݒ�
	const float targetHeight = 1.0f;//�^�[�Q�b�g�ʒu
	target = glm::vec3(0, targetHeight, 0) + player->pos;

	//�J�����⊮�ݒ�
	const float cameraSpeed = 0.1f;	//�J�����⊮�̑���
	pos = lastPos + (pos - lastPos) * cameraSpeed;
	target = lastTarget + (target - lastTarget) * cameraSpeed;

	//�ʒu�̕ۑ�
	lastPos = pos;
	lastTarget = target;
}

void Camera::draw()
{

	glMatrixMode(GL_PROJECTION);	// ���e�ϊ��s��̐ݒ�
	glLoadIdentity();				// �ϊ��s��̏�����

	//�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);
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