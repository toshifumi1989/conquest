#include "textureEffect.h"

/////////////////////////////////
//毎フレーム更新
/////////////////////////////////
void TextureEffect::update()
{
	pos += speed;

}


/////////////////////////////////
//描画（テクスチャ1枚
/////////////////////////////////
void TextureEffect::draw(int _textureID)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[_textureID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1);
		glVertex3d(-1, -1, 0);
		glTexCoord2d(1, 1);
		glVertex3d(1, -1, 0);
		glTexCoord2d(1, 0);
		glVertex3d(1, 1, 0);
		glTexCoord2d(0, 0);
		glVertex3d(-1, 1, 0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

}




