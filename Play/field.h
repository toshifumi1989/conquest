#ifndef FIELD_H
#define FIELD_H

#include "texture.h"
#include "../glm/glm.hpp"

class Field : public Texture
{
public:
	Field() :
		vtx(256),
		size(vtx)
	{
		center = glm::vec3(size / 2,0,size / 2);	
	}
	~Field() {}

	void setup(const char* _texture);	//�`�揀��
	void draw();						//�`��
	float intersect(glm::vec3 _pos);	//�����蔻�� y�����Ƃ�i�L�����N�^�[
	bool hitBullet(glm::vec3 _pos);		//�����蔻��i�o���b�g

	glm::vec3 center;					//�t�B�[���h�̒��S���W

private:
	int vtx;				//�t�B�[���h�̃s�N�Z����
	int size;				//�t�B�[���h�̍L��

};

extern Field *field;

#endif