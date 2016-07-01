#ifndef POLE_H
#define POLE_H

#include <vector>
#include "gameObject.h"

class Pole : public GameObject
{
public:
	Pole(glm::vec3 _pos, bool _onPlayer) :
		maxHP(100),
		HP(100),
		radius(2),
		height(20),
		sides(30),
		color(0,0,0)
	{
		pos = _pos;
		onPlayer = _onPlayer;
	}
	~Pole(){}

	void update();
	void draw();

	bool onPlayer;
	float maxHP;
	float HP;
	float radius;
	float height;
	unsigned int sides;

	glm::vec3 color;

};

extern std::vector< Pole* >pole;

#endif