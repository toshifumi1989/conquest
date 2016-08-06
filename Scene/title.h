#ifndef TITLE_H
#define TITLE_H

#include "scene.h"
#include "../glm/glm.hpp"

class Title :public Scene
{
public:
	Title():
		maskAlpha(0),
		onChangeScene(false)
	{}
	~Title(){}


	void init();			//初期設定	
	void update();			//毎フレーム更新
	void draw();			//描画
	void backGround();		//背景描画
	void word(int _textureID, glm::vec3 _translate);//描画文字
	void mask();			//マスク
	void pDelete();			//削除	
	bool changeScene();		//シーン切り替え

private:
	float maskAlpha;		//画面を黒にするために使う変数（マスクを作るため
	bool onChangeScene;		//プレイヤーがシーンの遷移を選択したらtrue
};

#endif