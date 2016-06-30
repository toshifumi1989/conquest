#include "manager.h"

#include "../scene/title.h"
#include "../scene/play.h"
#include "../scene/result.h"


Manager* Manager::instance = nullptr;
extern bool keys[256];

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


	if (keys[0x0d] == 1)
	{
		scene->pDelete();
		delete scene;
		_scene.change(&Manager::sceneTitle);
	}

}

