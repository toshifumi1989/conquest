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
		attackCount(120)
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

	void update();				//���t���[���X�V
	void draw();				//�`��
	void action();				//�s���i�ړ��A�U��
	bool onDead();				//�����m�F true:���S false:����
	int HP;						//�ϋv�l

private:
	void move(float _distance, unsigned int _onAttack);		//�ړ�
	void attack(float _distance, unsigned int _onAttack);	//�U��
	unsigned int type;			//�����w�c
	glm::vec3 searchTarget();	//�^�[�Q�b�g�̈ʒu���m�F
	glm::vec3 color;			//�`��̐F
	glm::vec3 targetPos;		//�^�[�Q�b�g�̈ʒu	
	int targetID;				//�^�[�Q�b�g�̔ԍ�
	int attackCount;			//�U���Ԋu�̂��߂̕ϐ�
	unsigned int maxHP;			//�ő�ϋv�l
};

extern std::list< NPC* > enemy;
extern std::list< NPC* > supporter;

#endif