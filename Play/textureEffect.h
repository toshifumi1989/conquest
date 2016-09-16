#ifndef TEXTUREEFFECT_H
#define TEXTUREEFFECT_H

#include "gameObject.h"
#include "texture.h"

/////////////////////////////////////
//�e�N�X�`���ł̃G�t�F�N�g�p�N���X
/////////////////////////////////////
class TextureEffect : public GameObject
{
public:
	TextureEffect() :
		alpha(1),
		alphaSpeed(0),
		scale(0,0),
		scaleSpeed(0,0)
	{}
	~TextureEffect(){}

	void update();
	void draw(char _textureID,glm::vec3 _rgb);

	float alpha;			//�A���t�@�l
	float alphaSpeed;		//�A���t�@�̕ύX���x
	glm::vec2 scale;		//�傫��
	glm::vec2 scaleSpeed;	//�傫���ύX���x

};



#endif