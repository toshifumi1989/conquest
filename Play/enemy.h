#ifndef ENEMY_H
#define ENEMY_H

#include <list>
#include "gameObject.h"

class Enemy :public GameObject
{
public:
	Enemy(glm::vec3 _pos, float _size, float _yaw) :
		color(0.7f, 0.2f, 0.1f),
		attackCount(120)
	{
		pos = _pos;
		lastPos = _pos;
		size = _size;
		yaw = _yaw;
		targetPos = searchTarget();
	}
	~Enemy() {}

	void update();				//���t���[���X�V
	void draw();				//�`��
	void action();				//�s���i�ړ��A�U��
	void move(float _distance, unsigned int _onAttack);				//�ړ�
	void attack(float _distance, unsigned int _onAttack);				//�U��

	glm::vec3 searchTarget();	//�^�[�Q�b�g�̈ʒu���m�F



private:
	const glm::vec3 color;		//�`��̐F
	glm::vec3 targetPos;		//�^�[�Q�b�g�̈ʒu	
	int targetID;				//�^�[�Q�b�g�̔ԍ�
	int attackCount;	//�U���Ԋu�̂��߂̕ϐ�
};

extern std::list< Enemy* > enemy;

#endif