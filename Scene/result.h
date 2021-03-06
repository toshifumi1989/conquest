#ifndef RESULT_H
#define RESULT_H

#include "scene.h"
#include "../glm/glm.hpp"

class Result :public Scene
{
public:
	Result(){}
	~Result(){}

private:
	void init();			//初期設定
	void update();			//毎フレーム更新
	void draw();			//描画
	void background();		//描画背景
	void word(int _textureID, glm::vec3 _translate);//描画文字
	void pDelete();			//削除
	bool changeScene();		//シーン切り替え
};


#endif