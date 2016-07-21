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
		chargeGauge(0),
		maxChargeGauge(50),
		maxHP(500),
		HP(500),
		bulletTargetPoint(0,0,0)
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
	void move();					//�L�����N�^�[�ړ�
	void moveLimit();				//�ړ������ꏊ�̓����蔻
	void poleCollision();			//�~���Ƃ̏Փ˔���
	void NPCCollision(std::list< NPC* > _NPC);//NPC�Ƃ̏Փ˔���
	void attack();					//�U��
	void bulletTarget();			//�e�̃^�[�Q�b�g
	bool isDead();					//true ���S
	void draw();					//�`��
	void bodyDraw();				//�̂̕`��
	void trajectoryDraw();			//�e���`��

	void HUD();						//�w�b�h�A�b�v�f�B�X�v���C
	void bulletChargeGauge();		//�V���b�g�Q�[�W
	void HPGauge();					//HP�Q�[�W

	unsigned int playerType();		//�v���C���[�̏�����n��

	int HP;							//�ϋv�l

private:
	glm::vec3 color;				//�L�����N�^�[�̐F
	unsigned int type;				//�����w�c
	int chargeGauge;				//�V���b�g�`���[�W��
	const int maxChargeGauge;		//�V���b�g�Q�[�W�̍ő�l
	const int maxHP;				//�ő�ϋv�l
	glm::vec3 bulletTargetPoint;	//�e�̏I���_
};

extern Player *player;
extern Texture *mark;

#endif