#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../glm/glm.hpp"

//�����w�c
enum TYPE
{
	NEUTRAL,	//����
	BLUE,		//�u���[�`�[��
	RED,		//���b�h�`�[��
};

////////////////////////////////////
//�Q�[���I�u�W�F�N�g�N���X
////////////////////////////////////
class GameObject 
{
public:
	GameObject() :
		pos(0, 0, 0),
		lastPos(0, 0, 0),
		speed(0, 0, 0),
		size(1),
		yaw(0)
	{}
	virtual ~GameObject() {}

	glm::vec3 pos;		//���ݒn
	glm::vec3 lastPos;	//�P�t���[���O�̈ʒu
	glm::vec3 speed;	//�ړ�����
	float yaw;			//�I�u�W�F�N�g�̌���

protected:
	float size;			//�I�u�W�F�N�g�̑傫��
};

#endif