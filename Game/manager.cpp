#include <stdio.h>

#include "manager.h"
#include "../scene/title.h"
#include "../scene/play.h"
#include "../scene/result.h"


Manager* Manager::instance = nullptr;

//////////////////////////////////////
//インスタンス
//////////////////////////////////////
Manager* Manager::getInstance()
{
	if (nullptr == instance)
	{
		instance = new Manager();

	}
	return instance;
}

////////////////////////////////////////////
//マネージャの更新
////////////////////////////////////////////
void Manager::update()
{
	_scene.run(this, 1.0f / 60.0f);
}


///////////////////////////////////////////
//タイトルシーン
///////////////////////////////////////////
void Manager::sceneTitle(float delta)
{
	if (_scene.getTime() == 0.0f)
	{
		scene = new Title();
		scene->init();
	}

	scene->update();
	scene->draw();

	if (scene->changeScene())
	{
		_scene.change(&Manager::scenePlay);
		scene->pDelete();
	}

}

//////////////////////////////////////////////
//プレイシーン
//////////////////////////////////////////////
void Manager::scenePlay(float delta)
{
	if (_scene.getTime() == 0.0f)
	{
		scene = new Play();
		scene->init();
	}

	scene->update();
	scene->draw();

	if (scene->changeScene())
	{
		_scene.change(&Manager::sceneResult);
		scene->pDelete();
	}
}

/////////////////////////////////////////////
//リザルトシーン
/////////////////////////////////////////////
void Manager::sceneResult(float delta)
{
	if (_scene.getTime() == 0.0f)
	{

		scene = new Result();
		scene->init();
	}

	scene->update();
	scene->draw();


	if (scene->changeScene())
	{
		scene->pDelete();
		_scene.change(&Manager::sceneTitle);
	}

}

