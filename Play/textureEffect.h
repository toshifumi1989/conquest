#ifndef TEXTUREEFFECT_H
#define TEXTUREEFFECT_H

#include "gameObject.h"
#include "texture.h"

class TextureEffect : public GameObject
{
public:
	TextureEffect() :
		alpha(1),
		alphaSpeed(0)
	{}
	~TextureEffect(){}

	void update();
	void draw(int _textureID);


private:
	float alpha;			//�A���t�@�l
	float alphaSpeed;		//�A���t�@�̕ύX���x
	unsigned int drawCount;	//�`��J�n����̃J�E���g
	bool onDraw;			//�`�悳��Ă��邩

	glm::vec2 scale;			//�傫��
	glm::vec2 scaleSpeed;		//�傫���ύX���x

};






#endif