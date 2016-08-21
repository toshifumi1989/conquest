#ifndef NPC_H
#define NPC_H

#include <list>
#include "gameObject.h"

class NPC :public GameObject
{
public:
	NPC(glm::vec3 _pos, float _size, float _yaw,unsigned char _type) :
		maxHP(500),
		HP(500),
		attackCount(120),
		stepCount(120),
		isDeadTimer(0),
		isDeathEffect(false)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		type = _type;
		if (_type == TYPE::BLUE)
		{//�u���[�w�c
			color = glm::vec3 (0.2f, 0.4f, 0.9f);
		}
		else if(_type == TYPE::RED)
		{//���b�h�w�c
			color = glm::vec3 (0.9f, 0.2f, 0.1f);
		}
		else
		{//����
			color = glm::vec3(1, 1, 1);
		}

		targetPos = searchTarget();
	}
	~NPC() {}

	void update(std::list< NPC* > _NPC);//���t���[���X�V
	void draw();				//�`��
	bool isDead();				//�����m�F true:���S false:����
	void playerCollision();		//�v���C���[�Ƃ̓����蔻��
	void poleCollision();		//�~���Ƃ̏Փ˔���
	void NPCCollision(std::list< NPC* > _NPC);//NPC�Ƃ̏Փ˔���

	int HP;							//�ϋv�l
	unsigned int type;				//�����w�c
	unsigned int isDeadTimer;		//����ł���̎���

private:
	void move(float _distance, unsigned int _onAttack);		//�ړ�
	void attack(float _distance, unsigned int _onAttack);	//�U��
	
	glm::vec3 searchTarget();	//�^�[�Q�b�g�̈ʒu���m�F
	glm::vec3 enemyTarget(std::list< NPC* >_npc);	//�G�^�[�Q�b�g�̈ʒu
	glm::vec3 color;			//�`��̐F
	glm::vec3 targetPos;		//�^�[�Q�b�g�̈ʒu	
	int targetID;				//�^�[�Q�b�g�̔ԍ�
	int attackCount;			//�U���Ԋu�̂��߂̕ϐ�
	int stepCount;				//����s���̊Ԋu�̂��߂̕ϐ�
	unsigned int maxHP;			//�ő�ϋv�l
	bool isDeathEffect;			//���S�G�t�F�N�g�t���O
};

extern std::list< NPC* > enemy;
extern std::list< NPC* > supporter;

#endif