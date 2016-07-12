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
	Player(glm::vec3 _pos,float _size,float _yaw,unsigned int _type):
		chargeGauge(0),
		maxChargeGauge(100)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		type = _type;
		if (_type == TYPE::BLUE)
		{//�u���[�w�c
			color = glm::vec3(0.2f, 0.4f, 0.9f);
		}
		else if (_type == TYPE::RED)
		{//���b�h�w�c
			color = glm::vec3(0.9f, 0.2f, 0.1f);
		}
		else
		{//����
			color = glm::vec3(1, 1, 1);
		}
	}
	~Player(){}

	void update();					//�X�V
	void draw();					//�`��
	void move();					//�L�����N�^�[�ړ�
	void attack();					//�U��
	void HUD();						//�w�b�h�A�b�v�f�B�X�v���C
	void shootMarker();				//�V���b�g�}�[�J�[
	void bulletChargeGauge();		//�V���b�g�Q�[�W

private:
	glm::vec3 color;				//�L�����N�^�[�̐F
	unsigned int type;				//�����w�c

	int chargeGauge;			//�V���b�g�`���[�W��
	const int maxChargeGauge;			//�V���b�g�Q�[�W�̍ő�l
};

extern Player *player;
extern Texture *mark;

#endif