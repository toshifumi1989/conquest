#ifndef MANAGER_H
#define MANAGER_H

#include "sequence.h"
#include "../scene/scene.h"

class Manager 
{
public:

	~Manager() {}

	void update();//更新

	static Manager* getInstance();

	void sceneTitle(float delta);	//シーンタイトル
	void scenePlay(float delta);	//シーンプレイ
	void sceneResult(float delta);	//シーンリザルト

	Sequence<Manager> _scene;

	Scene *scene;

private:
	Manager() 
	{
		_scene.change(&Manager::sceneTitle);
	}

	static Manager* instance;
};

#endif