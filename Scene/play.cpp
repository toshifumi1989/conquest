#include "play.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/field.h"
#include "../Play/wall.h"
#include "../Play/player.h"
#include "../Play/bullet.h"
#include "../Play/pole.h"

/////////////////////////
//������
/////////////////////////
void Play::init()
{
	//�t�B�[���h-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::FIELD]);
	field = new Field();
	field->setup("field.bmp");

	//��---------------------------------------------------------------------------------------
	const float fieldToDistance = 120;

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::WALL]);
	Wall *northWall = new Wall(glm::vec3(field->center.x, 0, field->center.z + fieldToDistance), 0);	//z100�̕�
	northWall->read("wall.bmp");
	wall.push_back(northWall);

	Wall *southWall = new Wall(glm::vec3(field->center.x, 0, field->center.z - fieldToDistance), 0);	//z-100�̕�
	southWall->read("wall.bmp");
	wall.push_back(southWall);

	Wall *eastWall = new Wall(glm::vec3(field->center.x + fieldToDistance, 0, field->center.z), 90);		//x100�̕�
	eastWall->read("wall.bmp");
	wall.push_back(eastWall);

	Wall *westWall = new Wall(glm::vec3(field->center.x - fieldToDistance, 0, field->center.z), 90);	//z-100�̕�
	westWall->read("wall.bmp");
	wall.push_back(westWall);

	//�|�[��--------------------------------------------------------------------------------
	const int poleNum = 4;	//pole�̐�(pole * pole)
	bool type = true;		//true�F�v���C���[�@false�F�G
	for (int y = 0; y < poleNum; y++)
	{
		if (y >= 2)
		{
			type = false;
		}
		for (int x = 0; x < poleNum; x++)
		{
			Pole *subPole = new Pole(glm::vec3(field->center.x + (40 * x - 60), 0, field->center.z + (40 * y - 60)), type);
			pole.push_back(subPole);
		}
	}




	//�v���C���[--------------------------------------------------------------------------
	player = new Player(glm::vec3(field->center.x, 4, field->center.z - 100), 0.5f, 0);

	//�J����------------------------------------------------------------------------------
	camera = new Camera();
	const float posHeight = 2.0f;	//�J��������
	const float distance = 7.0f;	//�v���C���[�Ƃ̋���
	const float targetHeight = 1.0f;//�^�[�Q�b�g�ʒu
	camera->setUp(
		player->pos + glm::vec3(0, posHeight, distance),	//�J�����̏����ʒu
		player->pos + glm::vec3(0, targetHeight,0));		//�^�[�Q�b�g�ʒu

}


/////////////////////////
//�X�V
/////////////////////////
void Play::update()
{
	//�v���C���[--------------------------
	player->move();
	player->attack();
	player->update();

	//�e----------------------------------
	std::list< Bullet* >::iterator bulletIter = playerBullet.begin();
	while (bulletIter != playerBullet.end())
	{
		(*bulletIter)->update();	//�ړ��X�V
		(*bulletIter)->exist();		//���݊m�F

		//�����e�����݂��Ȃ��Ƃ�����
		if (!(*bulletIter)->onExistFlag)
		{
			bulletIter = playerBullet.erase(bulletIter);
			return;
		}
		bulletIter++;
	}

	//�~��---------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->update();
	}

	//�J����---------------------------------
	camera->update();
}


/////////////////////////
//�`��
/////////////////////////
void Play::draw()
{

	glColor3f(1, 1, 1);		//�F�̏�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�J�����ݒ�--------------------------------------
	camera->draw();


	//�v���C���[--------------------------------------
	player->draw();

	//�e----------------------------------------------
	std::list< Bullet* >::iterator bulletIter = playerBullet.begin();
	while (bulletIter != playerBullet.end())
	{
		(*bulletIter)->draw();
		bulletIter++;
	}

	printf("size = %d\n", playerBullet.size());

	//�t�B�[���h�i�n�ʕ`��--------------------------
	field->draw();

	//�Ǖ`��----------------------------------------
	for (int i = 0; i < wall.size(); i++)
	{
		wall[i]->draw();
	}

	//�~��------------------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->draw();
	}


}

/////////////////////////
//���ʕ`��
/////////////////////////
void Play::twoDimension()
{

}

/////////////////////////
//�폜
/////////////////////////
void Play::pDelete()
{

}
