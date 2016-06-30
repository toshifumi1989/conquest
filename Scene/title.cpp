#include "title.h"
#include "../Play/camera.h"
#include "../Play/texture.h"

Texture *background;


////////////////////////////////////
//タイトル画面初期設定
////////////////////////////////////
void Title::init()
{
	//カメラ生成
	camera = new Camera();

	//背景
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_BACKGROUND]);
	background = new Texture();
	background->read("title.bmp");


}

//////////////////////////////////
//毎フレーム更新
//////////////////////////////////
void Title::update()
{

}

/////////////////////////////////
//描画
/////////////////////////////////
void Title::draw()
{
	//RGB初期化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//カメラ平面設定
	camera->twoDimensionCamera();

	//タイトル画面背景
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::TITLE_BACKGROUND]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1);
		glVertex3d(camera->left, camera->bottom, 0);
		glTexCoord2d(1, 1);
		glVertex3d(camera->right, camera->bottom, 0);
		glTexCoord2d(1, 0);
		glVertex3d(camera->right, camera->top, 0);
		glTexCoord2d(0, 0);
		glVertex3d(camera->left, camera->top, 0);
	}
	glEnd();

	//glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

/////////////////////////////////
//削除
/////////////////////////////////
void Title::pDelete()
{

}