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
		{//’†—§
			HP = 0 ;
		}
		else if (_type == TYPE::BLUE)
		{//ƒuƒ‹[w‰c
			HP = maxHP;
		}
		else
		{//ƒŒƒbƒhw‰c
			HP = -maxHP;
		}

	}
	~Pole(){}

	void recovery();				//©“®‰ñ•œ
	void draw();					//•`‰æ
	void occupation();				//è—Ì‚·‚é

	int HP;						//‰~’Œ‚Ìw‰cƒQ[ƒW
	unsigned int type;			//Š‘®w‰c

private:
	const int maxHP;			//Å‘å‘Ï‹v’l
	const float radius;			//‰~’Œ‚Ì”¼Œa
	const float height;			//‰~’Œ‚Ì‚‚³
	const unsigned int sides;	//‰~’Œ‚Ì•ªŠ„”

	glm::vec3 color;			//F

};

extern std::vector< Pole* >pole;

#endif