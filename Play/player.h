#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include "gameObject.h"
#include "field.h"

/////////////////////////////////////
//�v���C���[�L�����N�^�[class
/////////////////////////////////////
class Player : public GameObject
{
public:
	Player(glm::vec3 _pos,float _size,float _yaw) :
		color(0.2f, 0.4f, 0.7f)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
	}
	~Player(){}

	void update();					//�X�V
	void draw();					//�`��
	void move();					//�L�����N�^�[�ړ�
	void attack();					//�U��

	glm::vec3 color;		//�L�����N�^�[�̐F
};

extern Player *player;

#endif