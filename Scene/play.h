#ifndef PLAY_H
#define PLAY_H

#include "scene.h"

/////////////////////////////
//ゲームプレイシーン
/////////////////////////////
class Play : public Scene
{
public:
	Play(){}
	~Play() {}

	void init();		//初期設定
	void update();		//更新
	void draw();		//描画
	void HUD();			//ヘッドアップディスプレイ
	void pDelete();		//削除
	bool changeScene();	//シーン変更の条件

};


#endif