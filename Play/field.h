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

	void setup(const char* _texture);	//�`�揀��
	void draw();						//�`��
	void intersect(glm::vec3 _pos);		//�����蔻��
	float charcterHeight;				//�����蔻��̐��l�̑ҋ@�ꏊ
	glm::vec3 center;					//�t�B�[���h�̒��S���W

private:
	int vtx;				//�t�B�[���h�̃s�N�Z����
	int size;				//�t�B�[���h�̍L��

};

extern Field *field;

#endif