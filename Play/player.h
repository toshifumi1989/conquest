#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include "gameObject.h"
#include "npc.h"
#include "field.h"

/////////////////////////////////////
//�v���C���[�L�����N�^�[class
/////////////////////////////////////
class Player : public GameObject
{
public:
	Player(glm::vec3 _pos,float _size,float _yaw,unsigned int _type):
		isCharge(false),
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
	void moveLimit();				//�ړ������ꏊ�̓����蔻
	void poleCollision();			//�~���Ƃ̏Փ˔���
	void NPCCollision(std::list< NPC* > _NPC);//NPC�Ƃ̏Փ˔���
	void attackSpace();				//�U��(�L�[�{�[�h
	void attackMouse(int _button, int _state);//�U��(�}�E�X
	void charge();					//isCharge:true �Ȃ�`���[�W����
	void HUD();						//�w�b�h�A�b�v�f�B�X�v���C
	void shootMarker();				//�V���b�g�}�[�J�[
	void bulletChargeGauge();		//�V���b�g�Q�[�W
	unsigned int playerTypa();		//�v���C���[�̏����𔻕ʂ���

private:
	glm::vec3 color;				//�L�����N�^�[�̐F
	unsigned int type;				//�����w�c
	bool isCharge;					//true:�`���[�W��
	int chargeGauge;				//�V���b�g�`���[�W��
	const int maxChargeGauge;		//�V���b�g�Q�[�W�̍ő�l
};

extern Player *player;
extern Texture *mark;

#endif