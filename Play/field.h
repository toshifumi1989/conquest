#ifndef FIELD_H
#define FIELD_H

#include "texture.h"
#include "../glm/glm.hpp"

class Field : public Texture
{
public:
	Field() :
		vtx(256),
		size(vtx),
		charcterHeight(0)
	{
		center = glm::vec3(size / 2,0,size / 2);	
	}
	~Field() {}

	void setup(const char* _texture);	//描画準備
	void draw();						//描画
	void intersect(glm::vec3 _pos);		//当たり判定
	float charcterHeight;				//当たり判定の数値の待機場所
	glm::vec3 center;					//フィールドの中心座標

private:
	int vtx;				//フィールドのピクセル数
	int size;				//フィールドの広さ

};

extern Field *field;

#endif