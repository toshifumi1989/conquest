#include "textureEffect.h"

/////////////////////////////////
//毎フレーム更新
/////////////////////////////////
void TextureEffect::update()
{
	pos += speed;
	alpha -= alphaSpeed;
	scale += scaleSpeed;
}

/////////////////////////////////
//描画（テクスチャ1枚
/////////////////////////////////
void TextureEffect::draw(char _textureID, glm::vec3 _rgb)
{

	glBindTexture(GL_TEXTURE_2D, textures[_textureID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glPushMatrix();
	{
		glColor4f(_rgb.r, _rgb.g, _rgb.b, alpha);

		//煙の描画を常に正面に向ける（ビルボード
		glm::mat4 view;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);
		glm::mat4 m = inverse(view);
		m[3][0] = m[3][1] = m[3][2] = 0;
		glMultMatrixf((float*)&m);

		glScalef(scale.x, scale.y, 0);
		glTranslatef(pos.x, pos.y, pos.z);

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
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}




