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
	Player(glm::vec3 _pos,float _size,float _yaw,unsigned int _type) 
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		type = _type;
		if (_type == TYPE::BLUE)
		{
			color = glm::vec3(0.2f, 0.4f, 0.7f);
		}
		else if (_type == TYPE::RED)
		{
			color = glm::vec3(0.7f, 0.2f, 0.1f);
		}
		else
		{
			color = glm::vec3(1, 1, 1);
		}
	}
	~Player(){}

	void update();					//�X�V
	void draw();					//�`��
	void move();					//�L�����N�^�[�ړ�
	void attack();					//�U��

private:
	glm::vec3 color;				//�L�����N�^�[�̐F
	unsigned int type;				//�����w�c

};

extern Player *player;

#endif