#include "play.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/field.h"
#include "../Play/wall.h"
#include "../Play/player.h"
#include "../Play/bullet.h"
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
			Pole *subPole = new Pole(glm::vec3(field->center.x + (40 * x - 60), 0, field->center.z + (40 * y - 60)), type);
			pole.push_back(subPole);
		}
	}




	//プレイヤー--------------------------------------------------------------------------
	player = new Player(glm::vec3(field->center.x, 4, field->center.z - 100), 0.5f, 0);

	//カメラ------------------------------------------------------------------------------
	camera = new Camera();
	const float posHeight = 2.0f;	//カメラ高さ
	const float distance = 7.0f;	//プレイヤーとの距離
	const float targetHeight = 1.0f;//ターゲット位置
	camera->setUp(
		player->pos + glm::vec3(0, posHeight, distance),	//カメラの初期位置
		player->pos + glm::vec3(0, targetHeight,0));		//ターゲット位置

}


/////////////////////////
//更新
/////////////////////////
void Play::update()
{
	//プレイヤー--------------------------
	player->move();
	player->attack();
	player->update();

	//弾----------------------------------
	std::list< Bullet* >::iterator bulletIter = playerBullet.begin();
	while (bulletIter != playerBullet.end())
	{
		(*bulletIter)->update();	//移動更新
		(*bulletIter)->exist();		//存在確認

		//もう弾が存在しないとき消去
		if (!(*bulletIter)->onExistFlag)
		{
			bulletIter = playerBullet.erase(bulletIter);
			return;
		}
		bulletIter++;
	}

	//円柱---------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->update();
	}

	//カメラ---------------------------------
	camera->update();
}


/////////////////////////
//描画
/////////////////////////
void Play::draw()
{

	glColor3f(1, 1, 1);		//色の初期化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//カメラ設定--------------------------------------
	camera->draw();


	//プレイヤー--------------------------------------
	player->draw();

	//弾----------------------------------------------
	std::list< Bullet* >::iterator bulletIter = playerBullet.begin();
	while (bulletIter != playerBullet.end())
	{
		(*bulletIter)->draw();
		bulletIter++;
	}

	printf("size = %d\n", playerBullet.size());

	//フィールド（地面描画--------------------------
	field->draw();

	//壁描画----------------------------------------
	for (int i = 0; i < wall.size(); i++)
	{
		wall[i]->draw();
	}

	//円柱------------------------------------------
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
