#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "field.h"
#include"../glm/gtx/intersect.hpp"

Field *field;

/////////////////////////////
//フィールド準備
/////////////////////////////
void Field::setup(const char* _texture)
{
	//イメージ読み込み-----------
	FILE *pFile = fopen(_texture, "rb");
	assert(pFile != NULL);

	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	RGB *pixels;

	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pFile);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);

	const auto size = bi.biWidth *bi.biHeight * sizeof(RGB);
	pixels = (RGB*)malloc(size);
	fread(pixels, size, 1, pFile);

	fclose(pFile);

	//画像の上下反転
	for (int p = 0; p < bi.biHeight / 2; p++)
	{
		for (int q = 0; q < bi.biWidth; q++)
		{
			RGB sub = pixels[bi.biWidth * p + q];
			pixels[bi.biWidth * p + q] = pixels[bi.biWidth *(bi.biHeight - 1 - p) + q];
			pixels[bi.biWidth *(bi.biHeight - 1 - p) + q] = sub;
		}
	}

	glTexImage2D(
		GL_TEXTURE_2D,		//GLenum target,
		0,					//GLint level,
		GL_RGB,				//GLint internalformat, 
		bi.biWidth, bi.biHeight,	//GLsizei width, GLsizei height, 
		0,					//GLint border, 
		GL_RGB,				//GLenum format, 
		GL_UNSIGNED_BYTE,	//GLenum type, 
		pixels);			//const GLvoid *pixels

	//----------------------------------------
	//データ保存------------------------------
	for (int z = 0; z < vtx; z++)
	{
		for (int x = 0; x < vtx; x++)
		{

			//texture
			tex.push_back((float)x / (vtx - 1));
			tex.push_back((float)z / (vtx - 1));

			//ハイトマップ
			const auto ratio = (pixels[vtx * z + x].r + pixels[vtx * z + x].g + pixels[vtx * z + x].b) / 255.f;//0~1に変更
			const auto fieldHeight = 0;	//フィールドの高さ上限

			const auto y = ratio * fieldHeight;


			//vertex
			vertex.push_back((float)x);
			vertex.push_back(y);
			vertex.push_back((float)z);

			//normal
			normal.push_back(0);
			normal.push_back(1);
			normal.push_back(0);
		}
	}

	for (int z = 0; z < vtx - 1; z++)
	{//行の数
		for (int x = 0; x < vtx - 1; x++)
		{//列の数

			//index
			index.push_back(vtx * z + x);
			index.push_back(vtx * z + x + vtx);
			index.push_back(vtx * z + x + 1);
			index.push_back(vtx * z + x + 1);
			index.push_back(vtx * z + x + vtx);
			index.push_back(vtx * z + x + vtx + 1);
		}
	}

	free(pixels);
}

////////////////////////////////////
//フィールド描画
////////////////////////////////////
void Field::draw()
{
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glColor3f(1, .5, 0);

		glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_ID::FIELD]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		const auto *v = vertex.data();
		const auto *n = normal.data();
		const auto *ind = index.data();
		const auto *t = tex.data();

		glVertexPointer(3, GL_FLOAT, 0, v);
		glNormalPointer(GL_FLOAT, 0, n);
		glTexCoordPointer(2, GL_FLOAT, 0, t);

		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, ind);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

/////////////////////////////////
//フィールドとの当たり判定
/////////////////////////////////
float  Field::intersect(glm::vec3 _pos)
{

	const glm::vec3 orig = glm::vec3(_pos.x, -1, _pos.z);	//始点で、直線上の任意の点
	const glm::vec3 dir = glm::vec3(0, 1, 0);				//直線の向きで、直線を定義する
	glm::vec3 vert0;								//以下の３点で三角形を定義
	glm::vec3 vert1;
	glm::vec3 vert2;
	glm::vec3 distance = glm::vec3(0, 0, 0);		//xメンバに、始点から交差点の距離が取得される

	const int temporaryX = _pos.x;	//x座標の整数部分
	const int temporaryZ = _pos.z;	//z座標の整数部分
	const int nowPosY = _pos.y;		//現在のy座標

	if ((_pos.x - temporaryX) + (_pos.z - temporaryZ) < 1)
	{

		//上の三角
		vert0 = glm::vec3(temporaryX, vertex[(vtx * temporaryZ + temporaryX) * 3 + 1], temporaryZ);
		vert1 = glm::vec3(temporaryX + 1, vertex[(vtx * temporaryZ + temporaryX + 1) * 3 + 1], temporaryZ);
		vert2 = glm::vec3(temporaryX, vertex[((vtx * (temporaryZ + 1)) + temporaryX) * 3 + 1], temporaryZ + 1);

		//当たっていれば距離を返す
		if (glm::intersectLineTriangle(orig, dir, vert0, vert1, vert2, distance))
		{
			return distance.x;
		}
	}
	else
	{
		//下の三角
		vert0 = glm::vec3(temporaryX + 1, vertex[(vtx * temporaryZ + temporaryX + 1) * 3 + 1], temporaryZ);
		vert1 = glm::vec3(temporaryX, vertex[((vtx * (temporaryZ + 1)) + temporaryX) * 3 + 1], temporaryZ + 1);
		vert2 = glm::vec3(temporaryX + 1, vertex[((vtx * (temporaryZ + 1)) + temporaryX + 1) * 3 + 1], temporaryZ + 1);

		//当たっていれば距離を返す
		if (glm::intersectLineTriangle(orig, dir, vert0, vert1, vert2, distance))
		{
			return distance.x;
		}
	}

	glm::vec3 intersect = orig + dir * distance.x;
	return intersect.y;
}

////////////////////////////////////////////
//フィールドと弾の当たり判定
////////////////////////////////////////////
bool Field::hitBullet(glm::vec3 _pos)
{
	const glm::vec3 orig = glm::vec3(_pos.x, -0.1f, _pos.z);	//始点で、直線上の任意の点
	const glm::vec3 dir = glm::vec3(0, 1, 0);					//直線の向きで、直線を定義する
	glm::vec3 vert0;											//以下の３点で三角形を定義
	glm::vec3 vert1;
	glm::vec3 vert2;
	glm::vec3 distance = glm::vec3(0, 0, 0);					//xメンバに、始点から交差点の距離が取得される

	const int temporaryX = _pos.x;
	const int temporaryZ = _pos.z;
	const int nowPosY = _pos.y;

	//上の三角
	if ((_pos.x - temporaryX) + (_pos.z - temporaryZ) < 1)
	{
		vert0 = glm::vec3(temporaryX, field->vertex[(vtx * temporaryZ + temporaryX) * 3 + 1], temporaryZ);
		vert1 = glm::vec3(temporaryX + 1, field->vertex[(vtx * temporaryZ + temporaryX + 1) * 3 + 1], temporaryZ);
		vert2 = glm::vec3(temporaryX, field->vertex[((vtx * (temporaryZ + 1)) + temporaryX) * 3 + 1], temporaryZ + 1);

		//当たっていればtrueで返す
		if (glm::intersectLineTriangle(orig, dir, vert0, vert1, vert2, distance))
		{
			if (_pos.y <= distance.x)
			{
				return true;
			}
		}
	}
	else
	{//下の三角

		vert0 = glm::vec3(temporaryX + 1, field->vertex[(vtx * temporaryZ + temporaryX + 1) * 3 + 1], temporaryZ);
		vert1 = glm::vec3(temporaryX, field->vertex[((vtx * (temporaryZ + 1)) + temporaryX) * 3 + 1], temporaryZ + 1);
		vert2 = glm::vec3(temporaryX + 1, field->vertex[((vtx * (temporaryZ + 1)) + temporaryX + 1) * 3 + 1], temporaryZ + 1);

		//当たっていればtrueで返す
		if (glm::intersectLineTriangle(orig, dir, vert0, vert1, vert2, distance))
		{
			if (_pos.y <= distance.x)
			{
				return true;
			}
		}
	}

	return false;
}

///////////////////////////////////////////////
//x軸フィールド外かどうか(true:外 false:内
///////////////////////////////////////////////
bool Field::outXaxisField(glm::vec3 _pos)
{
	if (_pos.x > field->center.x + field->edge || _pos.x < field->center.x - field->edge)
	{
		return true;
	}

	return false;
}

///////////////////////////////////////////
//z軸フィールド外かどうか
//////////////////////////////////////////
bool Field::outZaxisField(glm::vec3 _pos)
{
	if (_pos.z > field->center.z + field->edge || _pos.z < field->center.z - field->edge)	//z軸
	{
		return true;
	}

	return false;
}


