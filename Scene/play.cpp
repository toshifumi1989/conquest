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
//初期化
/////////////////////////
void Play::init()
{
	//テクスチャ読み込み---------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::MARK]);
	mark = new Texture();
	mark->read_alpha("mark.bmp");

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
			type = TYPE::BLUE;
		}
		else if (y == 1)
		{//真ん中
			type = TYPE::NEUTRAL;
		}
		else
		{//エネミー	側
			type = TYPE::RED;
		}
		for (int x = 0; x < poleNum; x++)
		{
			Pole *subPole = new Pole(glm::vec3(field->center.x + (polePos * x - polePos), 0, field->center.z + (polePos * y - polePos)), type);
			pole.push_back(subPole);
		}
	}

	//ブルーチーム----------------------------------------------------------------
	//プレイヤー
	glm::vec3 centerToPlayer(45, 4, -100);		//フィールド中心からの位置
	float playerSize = 0.5f;					//プレイヤーの大きさ
	player = new Player(field->center + centerToPlayer, playerSize, 0, TYPE::BLUE);

	//サポーター
	auto supporterNum = 3;			//サポーターの数
	auto supporterSize = 0.5f;		//サポーターの大きさ

	for (int i = 0; i < supporterNum; i++)
	{
		glm::vec3 centerToSupporter((i * 30) - 45, 4, -100);	//フィールド中心からの位置	
		NPC* subSuppoter = new NPC(field->center + centerToSupporter, supporterSize, 0, TYPE::BLUE);
		supporter.push_back(subSuppoter);
	}

	//レッドチーム----------------------------------------------------------------
	auto enemyNum = 4;				//エネミーの数
	auto enemySize = 0.5f;			//エネミーの大きさ

	for (int i = 0; i < enemyNum; i++)
	{
		glm::vec3 centerToEnemy((i * 30) - 45, 4, 100);			//フィールド中心からの位置
		NPC* subEnemy = new NPC(field->center + centerToEnemy, enemySize, 180, TYPE::RED);
		enemy.push_back(subEnemy);
	}

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
	//キャラクター--------------------------------------
	//ブルー------------------------------
	//プレイヤー
	player->move();
	player->attackSpace();
	player->update();


	//サポーター
	std::list< NPC* >::iterator supporterIter = supporter.begin();
	while (supporterIter != supporter.end())
	{
		(*supporterIter)->action(enemy);
		(*supporterIter)->update();

		if ((*supporterIter)->onDead())
		{
			//死亡エフェクト生成
			for (int i = 0; i < 15; i++)
			{
				DeadEffect* deadEffe = new DeadEffect((*supporterIter)->pos, (*supporterIter)->overColor());
				deadEffect.push_back(deadEffe);
			}

			//HPが0を下回ったら削除
			supporterIter = supporter.erase(supporterIter);

			//新しく生成
			glm::vec3 centerToSupporter(rand() % 60 - 30, 4, -100);		//フィールド中心からの位置
			float supporterSize = 0.5f;									//エネミーの大きさ
			NPC* sub = new NPC(field->center + centerToSupporter, supporterSize, 180, TYPE::BLUE);
			supporter.push_back(sub);

			continue;
		}
		supporterIter++;
	}

	//レッド----------------------------------
	//エネミー
	std::list< NPC* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->action(supporter);
		(*enemyIter)->update();

		if ((*enemyIter)->onDead())
		{
			//死亡エフェクト生成
			for (int i = 0; i < 15; i++)
			{
				DeadEffect* deadEffe = new DeadEffect((*enemyIter)->pos, (*enemyIter)->overColor());
				deadEffect.push_back(deadEffe);
			}

			//HPが0を下回ったら削除
			enemyIter = enemy.erase(enemyIter);

			//新しく生成
			glm::vec3 centerToEnemy(rand() % 60 - 30, 4, 100);		//フィールド中心からの位置
			float enemySize = 0.5f;									//エネミーの大きさ
			NPC* sub = new NPC(field->center + centerToEnemy, enemySize, 180, TYPE::RED);
			enemy.push_back(sub);

			continue;
		}
		enemyIter++;
	}

	//弾---------------------------------------------

	std::list< Bullet* >::iterator bulletIter = bullet.begin();
	while (bulletIter != bullet.end())
	{
		(*bulletIter)->update();	//移動更新
		(*bulletIter)->exist();		//存在確認

		//もう弾が存在しないとき消去
		if ((*bulletIter)->onExistFlag == false)
		{
			bulletIter = bullet.erase(bulletIter);
			continue;
		}
		bulletIter++;
	}

	//エフェクト----------------------------------
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

	//サポーター
	std::list< NPC* >::iterator supporterIter = supporter.begin();
	while (supporterIter != supporter.end())
	{
		(*supporterIter)->draw();
		supporterIter++;
	}

	//エネミー
	std::list< NPC* >::iterator enemyIter = enemy.begin();
	while (enemyIter != enemy.end())
	{
		(*enemyIter)->draw();
		enemyIter++;
	}
	//弾----------------------------------------------
	std::list< Bullet* >::iterator bulletIter = bullet.begin();
	while (bulletIter != bullet.end())
	{
		(*bulletIter)->draw();
		bulletIter++;
	}

	//エフェクト----------------------------------
	std::list< DeadEffect* >::iterator deadIter = deadEffect.begin();
	while (deadIter != deadEffect.end())
	{
		(*deadIter)->draw();
		deadIter++;
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

	//HUD-------------------------------------------
	HUD();
}

/////////////////////////
//平面描画
/////////////////////////
void Play::HUD()
{
	//カメラ設定-------------------------------------
	camera->HUD();

	//プレイヤー-------------------------------------
	player->HUD();


}

/////////////////////////
//削除
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
//シーン変更条件
/////////////////////////////
bool Play::changeScene()
{
	//現在の円柱のタイプの数を数える
	int bluePoleCount = 0;
	int redPoleCount = 0;

	for (int i = 0; i < pole.size(); i++)
	{//すべての円柱をタイプ別に分ける
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

