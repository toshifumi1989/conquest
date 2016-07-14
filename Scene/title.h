#ifndef TITLE_H
#define TITLE_H

#include "scene.h"

class Title :public Scene
{
public:
	Title():
		maskAlpha(0),
		onChangeScene(false)
	{}
	~Title(){}


	void init();
	void update();
	void draw();
	void pDelete();
	bool changeScene();

private:
	float maskAlpha;		//画面を黒にするために使う変数（マスクを作るため
	bool onChangeScene;		//プレイヤーがシーンの遷移を選択したらtrue
};

#endif