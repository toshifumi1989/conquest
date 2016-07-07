#ifndef POLE_H
#define POLE_H

#include <vector>
#include "gameObject.h"

class Pole : public GameObject
{
public:
	Pole(glm::vec3 _pos, unsigned int _type) :
		type(0),
		maxHP(1000),
		radius(2),
		height(20),
		sides(30),
		color(0,0,0)
	{
		pos = _pos;
		type = _type;
		if (_type == TYPE::NEUTRAL)
		{
			HP = maxHP / 2;
		}
		else if (_type == TYPE::BLUE)
		{
			HP = maxHP;
		}
		else
		{
			HP = 0;
		}

	}
	~Pole(){}

	void recovery();	//������
	void draw();		//�`��
	void occupation();	//��̂���


	int HP;					//�~���̐w�c�Q�[�W
	unsigned int type;		//�����w�c

private:
	const int maxHP;				//�ő�ϋv�l
	const float radius;				//�~���̔��a
	const float height;				//�~���̍���
	const unsigned int sides;		//�~���̕�����

	glm::vec3 color;		//�F

};

extern std::vector< Pole* >pole;

#endif