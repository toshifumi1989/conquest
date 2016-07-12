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
		edge(119)
	{
		center = glm::vec3(size / 2,0,size / 2);	
	}
	~Field() {}

	void setup(const char* _texture);	//描画準備
	void draw();						//描画
	float intersect(glm::vec3 _pos);	//当たり判定 y軸をとる（キャラクター
	bool hitBullet(glm::vec3 _pos);		//当たり判定（バレット
	bool outXaxisField(glm::vec3 _pos);	//x軸フィールド外かどうか
	bool outZaxisField(glm::vec3 _pos);	//z軸フィールド外かどうか

	glm::vec3 center;					//フィールドの中心座標
	unsigned int edge;					//フィールドの端

private:
	const int vtx;						//フィールドのピクセル数
	const int size;						//フィールドの広さ

};

extern Field *field;

#endif