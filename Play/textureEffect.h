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
	float alpha;			//アルファ値
	float alphaSpeed;		//アルファの変更速度
	unsigned int drawCount;	//描画開始からのカウント
	bool onDraw;			//描画されているか

	glm::vec2 scale;			//大きさ
	glm::vec2 scaleSpeed;		//大きさ変更速度

};






#endif