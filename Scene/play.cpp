#include "play.h"
#include "../Play/camera.h"
#include "../Play/texture.h"
#include "../Play/field.h"
#include "../Play/wall.h"
#include "../Play/player.h"
#include "../Play/npc.h"
#include "../Play/bullet.h"
#include "../Play/pole.h"
#include "../Play/damageEffect.h"
#include "../Play/deadEffect.h"
#include "../Play/time.h"
#include "../Play/wavFile.h"


/////////////////////////
//初期化
/////////////////////////
void Play::init()
{
	//タイム---------------------------------------------------------------------------------
	time = new Time();

	//BGM再生-------------------------------------------------------------------------------------
	sound->playMusic(SOUND::PLAY_BGM);

	//フィールド-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::FIELD]);
	field = new Field();
	field->setup("field.bmp");

	//壁---------------------------------------------------------------------------------------
	const float fieldToDistance = 120;

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::WALL]);

	Wall *northWall = new Wall(glm::vec3(field->center.x, 0, field->center.z + fieldToDistance), 0);	//z+120の壁
	Wall *southWall = new Wall(glm::vec3(field->center.x, 0, field->center.z - fieldToDistance), 0);	//z-120の壁
	Wall *eastWall = new Wall(glm::vec3(field->center.x + fieldToDistance, 0, field->center.z), 90);	//x+120の壁
	Wall *westWall = new Wall(glm::vec3(field->center.x - fieldToDistance, 0, field->center.z), 90);	//z-120の壁

	northWall->read("wall.bmp");	//壁テクスチャ読み込み

	wall.push_back(northWall);
	wall.push_back(southWall);
	wall.push_back(eastWall);
	wall.push_back(westWall);

	//スモークテクスチャ-------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::SMOKEEFFECT]);
	smokeTex = new Texture();
	smokeTex->read_alpha("smoke.bmp");

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
	
	//コントローラー
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::CONTROLLER]);
	controller = new Texture();
	controller->read_alpha("controller.bmp");

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
	//時計---------------------------------------------
	time->update();

	//キャラクター--------------------------------------
	const int countRePop = 60 * 5; //5秒後に再復帰
	//ブルー------------------------------
	//プレイヤー
	player->update();

	if (player->isDeadTimer >= countRePop)
	{//HPが0を下回ったら
		//いったん削除
		delete player;

		//新しく生成
		glm::vec3 centerToPlayer(rand() % 60 - 30, 4, -100);		//フィールド中心からの位置
		float playerSize = 0.5f;					//プレイヤーの大きさ
		player = new Player(field->center + centerToPlayer, playerSize, 0, TYPE::BLUE);

		//カメラの位置調整
		const float posHeight = 2.0f;	//カメラ高さ
		const float distance = -7.0f;	//プレイヤーとの距離
		const float targetHeight = 1.0f;//ターゲット位置

		camera->setUp(
			player->pos + glm::vec3(0, posHeight, distance),	//カメラの初期位置
			player->pos + glm::vec3(0, targetHeight, 0));		//ターゲット位置
	}

	//サポーター
	std::list< NPC* >::iterator supporterIter = supporter.begin();
	while (supporterIter != supporter.end())
	{
		(*supporterIter)->update(enemy);

		if ((*supporterIter)->isDeadTimer >= countRePop)
		{
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
		(*enemyIter)->update(supporter);

		if ((*enemyIter)->isDeadTimer >= countRePop)
		{
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
	std::list< DamageEffect* >::iterator damageIter = damageEffect.begin();
	while (damageIter != damageEffect.end())
	{
		(*damageIter)->update();
		if ((*damageIter)->count() == 0) damageIter = damageEffect.erase(damageIter);
		else damageIter++;
	}

	std::list< DeadEffect* >::iterator deadIter = deadEffect.begin();
	while (deadIter != deadEffect.end())
	{
		(*deadIter)->update();
		if ((*deadIter)->del()) deadIter = deadEffect.erase(deadIter);
		else deadIter++;
	}


	//円柱---------------------------------
	for (int i = 0; i < pole.size(); i++)
	{
		pole[i]->occupation();
		pole[i]->recovery();
	}

	//音楽の停止確認------------------------
	sound->playSceneStopMusic();

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
	std::list< DamageEffect* >::iterator damageIter = damageEffect.begin();
	while (damageIter != damageEffect.end())
	{
		(*damageIter)->draw();
		damageIter++;
	}

	std::list< DeadEffect* >::iterator deadIter = deadEffect.begin();
	while (deadIter != deadEffect.end())
	{
		(*deadIter)->draw();
		deadIter++;
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
	delete controller;
	delete field;
	delete time;
	delete smokeTex;

	wall.clear();
	enemy.clear();
	supporter.clear();
	bullet.clear();

	sound->stopMusic(SOUND::PLAY_BGM);
	sound->stopMusic(SOUND::SHOOT);
	sound->stopMusic(SOUND::ISDEAD);
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
		return true;


	if (time->getMinutes() < 0)
		return true;


	return false;
}

