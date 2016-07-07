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
	const char poleNum = 3;	//poleの数(pole * pole)
	const char polePos = 60;//中心位置からの距離
	unsigned char type;		//0:中立 1:プレイヤー 2:エネミー

	for (int y = 0; y < poleNum; y++)
	{
		if (y == 0)
		{//プレイヤー側
			type = TYPE::PLAYER;
		}
		else if (y == 1)
		{//真ん中
			type = TYPE::NEUTRAL;
		}
		else
		{//エネミー	側
			type = TYPE::ENEMY;
		}
		for (int x = 0; x < poleNum; x++)
		{
			Pole *subPole = new Pole(glm::vec3(field->center.x + (polePos * x - polePos), 0, field->center.z + (polePos * y - polePos)), type);
			pole.push_back(subPole);
		}
	}


	//プレイヤー--------------------------------------------------------------------------
	glm::vec3 centerToPlayer(0, 4, -100);	//フィールド中心からの位置
	float playerSize = 0.5f;				//プレイヤーの大きさ
	player = new Player(field->center + centerToPlayer, playerSize, 0);

	//エネミー---------------------------------------------------------------------------
	glm::vec3 centerToEnemy(30, 4, 100);		//フィールド中心からの位置
	float enemySize = 0.5f;					//エネミーの大きさ
	Enemy* subEnemy = new Enemy(field->center + centerToEnemy, enemySize, 180);
	enemy.push_back(subEnemy);

	//カメラ------------------------------------------------------------------------------
	camera = new Camera();
	const float posHeight = 2.0f;	//カメラ高さ
	const float distance = -7.0f;	//プレイヤーとの距離
	const float targetHeight = 1.0f;//ターゲット位置

	camera->setUp(
		player->pos + glm::vec3(0, posHeight, distance),	//カメラの初期位置
		player->pos + glm::vec3(0, targetHeight, 0));		//ターゲット位置

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

	//エネミー-----------------------------
	std::list< Enemy* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->action();
		(*enemyIter)->update();
		enemyIter++;
	}

	//弾----------------------------------
	//プレイヤー
	std::list< Bullet* >::iterator pbulletIter = playerBullet.begin();
	while (pbulletIter != playerBullet.end())
	{
		(*pbulletIter)->update();	//移動更新
		(*pbulletIter)->exist();		//存在確認

		//もう弾が存在しないとき消去
		if (!(*pbulletIter)->onExistFlag)
		{
			pbulletIter = playerBullet.erase(pbulletIter);
			return;
		}
		pbulletIter++;
	}

	//エネミー
	std::list< Bullet* >::iterator ebulletIter = enemyBullet.begin();
	while (ebulletIter != enemyBullet.end())
	{
		(*ebulletIter)->update();	//移動更新
		(*ebulletIter)->exist();		//存在確認

										//もう弾が存在しないとき消去
		if (!(*ebulletIter)->onExistFlag)
		{
			ebulletIter = enemyBullet.erase(ebulletIter);
			return;
		}
		ebulletIter++;
	}
	//円柱---------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->occupation();
		pole[i]->recovery();
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

	//キャラクター-------------------------------------
	//プレイヤー
	player->draw();

	//エネミー
	std::list< Enemy* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->draw();
		enemyIter++;
	}
	//弾----------------------------------------------
	//プレイヤー
	std::list< Bullet* >::iterator pbulletIter = playerBullet.begin();
	while (pbulletIter != playerBullet.end())
	{
		(*pbulletIter)->draw();
		pbulletIter++;
	}

	//エネミー
	std::list< Bullet* >::iterator ebulletIter = enemyBullet.begin();
	while (ebulletIter != enemyBullet.end())
	{
		(*ebulletIter)->draw();
		ebulletIter++;
	}

	//フィールド（地面描画--------------------------
	field->draw();

	//壁描画----------------------------------------
	for (int i = 0; i < wall.size(); i++)
	{

			wall[i]->draw(TEXTURE_ID::WALL);

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
