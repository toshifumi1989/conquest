#include <stdio.h>

#include "manager.h"
#include "../scene/title.h"
#include "../scene/play.h"
#include "../scene/result.h"


Manager* Manager::instance = nullptr;

//////////////////////////////////////
//�C���X�^���X
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
//�}�l�[�W���̍X�V
////////////////////////////////////////////
void Manager::update()
{
	_scene.run(this, 1.0f / 60.0f);
}


///////////////////////////////////////////
//�^�C�g���V�[��
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
//�v���C�V�[��
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
//���U���g�V�[��
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

