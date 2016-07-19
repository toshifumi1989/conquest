#include "play.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/field.h"
#include "../Play/wall.h"
#include "../Play/player.h"
#include "../Play/npc.h"
#include "../Play/bullet.h"
#include "../Play/pole.h"
#include "../Play/deadEffect.h"

/////////////////////////
//������
/////////////////////////
void Play::init()
{
	//�e�N�X�`���ǂݍ���---------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::MARK]);
	mark = new Texture();
	mark->read_alpha("mark.bmp");

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
	const char poleNum = 3;	//pole�̐�(pole * pole)
	const char polePos = 60;//���S�ʒu����̋���
	unsigned char type;		//0:���� 1:�v���C���[ 2:�G�l�~�[

	for (int y = 0; y < poleNum; y++)
	{
		if (y == 0)
		{//�v���C���[��
			type = TYPE::BLUE;
		}
		else if (y == 1)
		{//�^��
			type = TYPE::NEUTRAL;
		}
		else
		{//�G�l�~�[	��
			type = TYPE::RED;
		}
		for (int x = 0; x < poleNum; x++)
		{
			Pole *subPole = new Pole(glm::vec3(field->center.x + (polePos * x - polePos), 0, field->center.z + (polePos * y - polePos)), type);
			pole.push_back(subPole);
		}
	}

	//�u���[�`�[��----------------------------------------------------------------
	//�v���C���[
	glm::vec3 centerToPlayer(45, 4, -100);		//�t�B�[���h���S����̈ʒu
	float playerSize = 0.5f;					//�v���C���[�̑傫��
	player = new Player(field->center + centerToPlayer, playerSize, 0, TYPE::BLUE);

	//�T�|�[�^�[
	auto supporterNum = 3;			//�T�|�[�^�[�̐�
	auto supporterSize = 0.5f;		//�T�|�[�^�[�̑傫��

	for (int i = 0; i < supporterNum; i++)
	{
		glm::vec3 centerToSupporter((i * 30) - 45, 4, -100);	//�t�B�[���h���S����̈ʒu	
		NPC* subSuppoter = new NPC(field->center + centerToSupporter, supporterSize, 0, TYPE::BLUE);
		supporter.push_back(subSuppoter);
	}

	//���b�h�`�[��----------------------------------------------------------------
	auto enemyNum = 4;				//�G�l�~�[�̐�
	auto enemySize = 0.5f;			//�G�l�~�[�̑傫��

	for (int i = 0; i < enemyNum; i++)
	{
		glm::vec3 centerToEnemy((i * 30) - 45, 4, 100);			//�t�B�[���h���S����̈ʒu
		NPC* subEnemy = new NPC(field->center + centerToEnemy, enemySize, 180, TYPE::RED);
		enemy.push_back(subEnemy);
	}

	//�J����------------------------------------------------------------------------------
	camera = new Camera();
	const float posHeight = 2.0f;	//�J��������
	const float distance = -7.0f;	//�v���C���[�Ƃ̋���
	const float targetHeight = 1.0f;//�^�[�Q�b�g�ʒu

	camera->setUp(
		player->pos + glm::vec3(0, posHeight, distance),	//�J�����̏����ʒu
		player->pos + glm::vec3(0, targetHeight, 0));		//�^�[�Q�b�g�ʒu

}


/////////////////////////
//�X�V
/////////////////////////
void Play::update()
{
	//�L�����N�^�[--------------------------------------
	//�u���[------------------------------
	//�v���C���[
	player->move();
	player->attackSpace();
	player->update();


	//�T�|�[�^�[
	std::list< NPC* >::iterator supporterIter = supporter.begin();
	while (supporterIter != supporter.end())
	{
		(*supporterIter)->action(enemy);
		(*supporterIter)->update();

		if ((*supporterIter)->onDead())
		{
			//���S�G�t�F�N�g����
			for (int i = 0; i < 15; i++)
			{
				DeadEffect* deadEffe = new DeadEffect((*supporterIter)->pos, (*supporterIter)->overColor());
				deadEffect.push_back(deadEffe);
			}

			//HP��0�����������폜
			supporterIter = supporter.erase(supporterIter);

			//�V��������
			glm::vec3 centerToSupporter(rand() % 60 - 30, 4, -100);		//�t�B�[���h���S����̈ʒu
			float supporterSize = 0.5f;									//�G�l�~�[�̑傫��
			NPC* sub = new NPC(field->center + centerToSupporter, supporterSize, 180, TYPE::BLUE);
			supporter.push_back(sub);

			continue;
		}
		supporterIter++;
	}

	//���b�h----------------------------------
	//�G�l�~�[
	std::list< NPC* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->action(supporter);
		(*enemyIter)->update();

		if ((*enemyIter)->onDead())
		{
			//���S�G�t�F�N�g����
			for (int i = 0; i < 15; i++)
			{
				DeadEffect* deadEffe = new DeadEffect((*enemyIter)->pos, (*enemyIter)->overColor());
				deadEffect.push_back(deadEffe);
			}

			//HP��0�����������폜
			enemyIter = enemy.erase(enemyIter);

			//�V��������
			glm::vec3 centerToEnemy(rand() % 60 - 30, 4, 100);		//�t�B�[���h���S����̈ʒu
			float enemySize = 0.5f;									//�G�l�~�[�̑傫��
			NPC* sub = new NPC(field->center + centerToEnemy, enemySize, 180, TYPE::RED);
			enemy.push_back(sub);

			continue;
		}
		enemyIter++;
	}

	//�e---------------------------------------------

	std::list< Bullet* >::iterator bulletIter = bullet.begin();
	while (bulletIter != bullet.end())
	{
		(*bulletIter)->update();	//�ړ��X�V
		(*bulletIter)->exist();		//���݊m�F

		//�����e�����݂��Ȃ��Ƃ�����
		if ((*bulletIter)->onExistFlag == false)
		{
			bulletIter = bullet.erase(bulletIter);
			continue;
		}
		bulletIter++;
	}

	//�G�t�F�N�g----------------------------------
	std::list< DeadEffect* >::iterator deadIter = deadEffect.begin();
	while (deadIter != deadEffect.end())
	{
		(*deadIter)->update();
		if ((*deadIter)->count() == 0)
		{
			deadIter = deadEffect.erase(deadIter);
			continue;
		}
		deadIter++;
	}


	//�~��---------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->occupation();
		pole[i]->recovery();
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

	//�L�����N�^�[-------------------------------------
	//�v���C���[
	player->draw();

	//�T�|�[�^�[
	std::list< NPC* >::iterator supporterIter = supporter.begin();
	while (supporterIter != supporter.end())
	{
		(*supporterIter)->draw();
		supporterIter++;
	}

	//�G�l�~�[
	std::list< NPC* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->draw();
		enemyIter++;
	}
	//�e----------------------------------------------
	std::list< Bullet* >::iterator bulletIter = bullet.begin();
	while (bulletIter != bullet.end())
	{
		(*bulletIter)->draw();
		bulletIter++;
	}

	//�G�t�F�N�g----------------------------------
	std::list< DeadEffect* >::iterator deadIter = deadEffect.begin();
	while (deadIter != deadEffect.end())
	{
		(*deadIter)->draw();
		deadIter++;
	}
	//�t�B�[���h�i�n�ʕ`��--------------------------
	field->draw();

	//�Ǖ`��----------------------------------------
	for (int i = 0; i < wall.size(); i++)
	{
		wall[i]->draw(TEXTURE_ID::WALL);
	}

	//�~��------------------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->draw();
	}

	//HUD-------------------------------------------
	HUD();
}

/////////////////////////
//���ʕ`��
/////////////////////////
void Play::HUD()
{
	//�J�����ݒ�-------------------------------------
	camera->HUD();

	//�v���C���[-------------------------------------
	player->HUD();


}

/////////////////////////
//�폜
/////////////////////////
void Play::pDelete()
{
	delete player;
	delete mark;
	delete field;

	wall.clear();
	enemy.clear();
	supporter.clear();
	bullet.clear();
}

/////////////////////////////
//�V�[���ύX����
/////////////////////////////
bool Play::changeScene()
{
	//���݂̉~���̃^�C�v�̐��𐔂���
	int bluePoleCount = 0;
	int redPoleCount = 0;

	for (int i = 0; i < pole.size(); i++)
	{//���ׂẲ~�����^�C�v�ʂɕ�����
		if (pole[i]->type == TYPE::BLUE)
			bluePoleCount++;

		if (pole[i]->type == TYPE::RED)
			redPoleCount++;
	}

	if (bluePoleCount == pole.size() ||
		redPoleCount == pole.size())
	{
		return true;
	}

	return false;
}

