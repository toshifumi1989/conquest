#ifndef TEXTUREEFFECT_H
#define TEXTUREEFFECT_H

#include "gameObject.h"
#include "texture.h"

/////////////////////////////////////
//テクスチャでのエフェクト用クラス
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

	float alpha;			//アルファ値
	float alphaSpeed;		//アルファの変更速度
	glm::vec2 scale;		//大きさ
	glm::vec2 scaleSpeed;	//大きさ変更速度

};



#endif