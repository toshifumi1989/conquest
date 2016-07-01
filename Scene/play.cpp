#include "play.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/field.h"
#include "../Play/wall.h"
#include "../Play/player.h"
#include "../Play/pole.h"

/////////////////////////
//初期化
/////////////////////////
void Play::init()
{
	//フィールド-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::FIELD]);
	field = new Field();
	field->setup("field.bmp");

	//壁---------------------------------------------------------------------------------------
	const float fieldToDistance = 120;

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::WALL]);
	Wall *northWall = new Wall(glm::vec3(field->center.x, 0, field->center.z + fieldToDistance), 0);	//z100の壁
	northWall->read("wall.bmp");
	wall.push_back(northWall);

	Wall *southWall = new Wall(glm::vec3(field->center.x, 0, field->center.z - fieldToDistance), 0);	//z-100の壁
	southWall->read("wall.bmp");
	wall.push_back(southWall);

	Wall *eastWall = new Wall(glm::vec3(field->center.x + fieldToDistance, 0, field->center.z), 90);		//x100の壁
	eastWall->read("wall.bmp");
	wall.push_back(eastWall);

	Wall *westWall = new Wall(glm::vec3(field->center.x - fieldToDistance, 0, field->center.z), 90);	//z-100の壁
	westWall->read("wall.bmp");
	wall.push_back(westWall);

	//ポール--------------------------------------------------------------------------------
	const int poleNum = 4;	//poleの数(pole * pole)
	bool type = true;		//true：プレイヤー　false：敵
	for (int y = 0; y < poleNum; y++)
	{
		if (y >= 2)
		{
			type = false;
		}
		for (int x = 0; x < poleNum; x++)
		{
			Pole *subPole = new Pole(glm::vec3(field->center.x + ( 40 * x - 60) , 0, field->center.z + (40 * y - 60)), type);
			pole.push_back(subPole);
		}
	}




	//プレイヤー--------------------------------------------------------------------------
	player = new Player(glm::vec3(field->center.x, 4, field->center.z - 100), 0.5f, 0);

	//カメラ------------------------------------------------------------------------------
	camera = new Camera();
	const float cameraHeight = 2.0f;
	camera->setUp(glm::vec3(0, player->pos.y + cameraHeight, camera->distance), player->pos);

}


/////////////////////////
//更新
/////////////////////////
void Play::update()
{
	player->move();
	player->update();

	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->update();
	}

	camera->update();
}


/////////////////////////
//描画
/////////////////////////
void Play::draw()
{

	glColor3f(1, 1, 1);		//色の初期化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	camera->draw();

	field->draw();

	player->draw();

	for (int i = 0; i < wall.size(); i++)
	{
		wall[i]->draw();
	}


	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->draw();
	}

}

/////////////////////////
//平面描画
/////////////////////////
void Play::twoDimension()
{

}

/////////////////////////
//削除
/////////////////////////
void Play::pDelete()
{

}
