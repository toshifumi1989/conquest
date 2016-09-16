#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <vector>
#include "../glut.h"

enum TEXTURE_ID 
{//�ǂݍ��݃e�N�X�`���ԍ�
	TITLE_BACKGROUND,	//�^�C�g���q�[
	TITLE_SPACE,		//�^�C�g�����앶��
	TITLE_NAME,			//�^�C�g����

	FIELD,				//�t�B�[���h
	WALL,				//��
	CONTROLLER,			//������@
	SMOKEEFFECT,		//���G�t�F�N�g�p

	VICTORYorDEFEAT,	//���s
	RESULT_WORD,		//���U���g���앶��

	TEXTURE_MAX,		//�e�N�X�`��������
};

class Texture
{
public:
	Texture() {}
	~Texture(){}

	void read(const char* _texture);		//�ǂݍ���
	void read_alpha(const char* _texture);	//�ǂݍ��݁i�A���t�@�ϊ�

protected:

	typedef struct { unsigned char b, g, r; }RGB;	//�F��RGB�̒l
	std::vector<unsigned char>alpha;//�A���t�@�l

	std::vector<float>vertex;		//�ʒu
	std::vector<unsigned int>index;	//�ԍ�
	std::vector<float>normal;		//�@��
	std::vector<float>tex;			//�e�N�X�`���ԍ�

};

extern GLuint textures[TEXTURE_ID::TEXTURE_MAX];
extern Texture *smokeTex;

#endif