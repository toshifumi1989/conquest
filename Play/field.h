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

	void setup(const char* _texture);	//�`�揀��
	void draw();						//�`��
	float intersect(glm::vec3 _pos);	//�����蔻�� y�����Ƃ�i�L�����N�^�[
	bool hitBullet(glm::vec3 _pos);		//�����蔻��i�o���b�g
	bool outXaxisField(glm::vec3 _pos);	//x���t�B�[���h�O���ǂ���
	bool outZaxisField(glm::vec3 _pos);	//z���t�B�[���h�O���ǂ���

	glm::vec3 center;					//�t�B�[���h�̒��S���W
	unsigned int edge;					//�t�B�[���h�̒[

private:
	const int vtx;						//�t�B�[���h�̃s�N�Z����
	const int size;						//�t�B�[���h�̍L��

};

extern Field *field;

#endif