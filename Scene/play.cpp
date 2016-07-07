#include "play.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/field.h"
#include "../Play/wall.h"
#include "../Play/player.h"
#include "../Play/enemy.h"
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
	const char poleNum = 3;	//pole�̐�(pole * pole)
	const char polePos = 60;//���S�ʒu����̋���
	unsigned char type;		//0:���� 1:�v���C���[ 2:�G�l�~�[

	for (int y = 0; y < poleNum; y++)
	{
		if (y == 0)
		{//�v���C���[��
			type = TYPE::PLAYER;
		}
		else if (y == 1)
		{//�^��
			type = TYPE::NEUTRAL;
		}
		else
		{//�G�l�~�[	��
			type = TYPE::ENEMY;
		}
		for (int x = 0; x < poleNum; x++)
		{
			Pole *subPole = new Pole(glm::vec3(field->center.x + (polePos * x - polePos), 0, field->center.z + (polePos * y - polePos)), type);
			pole.push_back(subPole);
		}
	}


	//�v���C���[--------------------------------------------------------------------------
	glm::vec3 centerToPlayer(0, 4, -100);	//�t�B�[���h���S����̈ʒu
	float playerSize = 0.5f;				//�v���C���[�̑傫��
	player = new Player(field->center + centerToPlayer, playerSize, 0);

	//�G�l�~�[---------------------------------------------------------------------------
	glm::vec3 centerToEnemy(30, 4, 100);		//�t�B�[���h���S����̈ʒu
	float enemySize = 0.5f;					//�G�l�~�[�̑傫��
	Enemy* subEnemy = new Enemy(field->center + centerToEnemy, enemySize, 180);
	enemy.push_back(subEnemy);

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
	//�v���C���[--------------------------
	player->move();
	player->attack();
	player->update();

	//�G�l�~�[-----------------------------
	std::list< Enemy* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->action();
		(*enemyIter)->update();
		enemyIter++;
	}

	//�e----------------------------------
	//�v���C���[
	std::list< Bullet* >::iterator pbulletIter = playerBullet.begin();
	while (pbulletIter != playerBullet.end())
	{
		(*pbulletIter)->update();	//�ړ��X�V
		(*pbulletIter)->exist();		//���݊m�F

		//�����e�����݂��Ȃ��Ƃ�����
		if (!(*pbulletIter)->onExistFlag)
		{
			pbulletIter = playerBullet.erase(pbulletIter);
			return;
		}
		pbulletIter++;
	}

	//�G�l�~�[
	std::list< Bullet* >::iterator ebulletIter = enemyBullet.begin();
	while (ebulletIter != enemyBullet.end())
	{
		(*ebulletIter)->update();	//�ړ��X�V
		(*ebulletIter)->exist();		//���݊m�F

										//�����e�����݂��Ȃ��Ƃ�����
		if (!(*ebulletIter)->onExistFlag)
		{
			ebulletIter = enemyBullet.erase(ebulletIter);
			return;
		}
		ebulletIter++;
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

	//�G�l�~�[
	std::list< Enemy* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->draw();
		enemyIter++;
	}
	//�e----------------------------------------------
	//�v���C���[
	std::list< Bullet* >::iterator pbulletIter = playerBullet.begin();
	while (pbulletIter != playerBullet.end())
	{
		(*pbulletIter)->draw();
		pbulletIter++;
	}

	//�G�l�~�[
	std::list< Bullet* >::iterator ebulletIter = enemyBullet.begin();
	while (ebulletIter != enemyBullet.end())
	{
		(*ebulletIter)->draw();
		ebulletIter++;
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
