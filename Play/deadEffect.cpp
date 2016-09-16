#include "deadEffect.h"
#include "../glut.h"

std::list< DeadEffect* >deadEffect;

void DeadEffect::init(glm::vec3 _pos)
{
	//deadEffect自体の位置
	pos = _pos;

	//煙部分
	for (int i = 0; i < 10; i++)
	{
		TextureEffect smk;

		smk.alpha = 1.0f - rand() % 3 / 10.f;
		smk.alphaSpeed = 0.01f + rand() % 3 / 1000.f;

		smk.pos = glm::vec3(
			rand() % 10 / 10.f - 0.5f,
			rand() % 10 / 10.f - 0.5f,
			rand() % 10 / 10.f - 0.5f);
		smk.speed = glm::vec3(
			(-0.5f + rand() % 10 / 10.f) / 50.f,
			0.004f + rand() % 2 / 1000.f,
			(-0.5f + rand() % 10 / 10.f) / 50.f);

		smk.scale = glm::vec2(rand() % 10 / 5, rand() % 10 / 5);
		smk.scaleSpeed = glm::vec2(0.05f + rand() % 5 / 1000.f, 0.05f + rand() % 5 / 1000.f);

		smoke.push_back(smk);
	}

	//炎部分
	for (int i = 0; i < 7; i++)
	{
		TextureEffect tmpFire;

		tmpFire.alpha = 1.1f - rand() % 3 / 10.f;
		tmpFire.alphaSpeed = 0.03 + rand() % 2 / 100.f;

		tmpFire.pos = glm::vec3(
			rand() % 10 / 10.f - 0.5f, 
			rand() % 10 / 10.f - 0.5f, 
			rand() % 10 / 10.f - 0.5f);
		tmpFire.speed = glm::vec3(0, 0, 0);

		tmpFire.scale = glm::vec2(rand() % 4 / 2000.f, rand() % 4 / 2000.f);
		tmpFire.scaleSpeed = glm::vec2(0.1f + rand() % 20 / 500.f, 0.1f + rand() % 20 / 500.f);

		fire.push_back(tmpFire);
	}
	
}

//////////////////////////////////////
//マイフレーム更新
//////////////////////////////////////
void DeadEffect::update()
{
	//煙更新---------------------------------------------------------
	std::list< TextureEffect >::iterator smokeIter = smoke.begin();
	while (smokeIter != smoke.end())
	{
		smokeIter->update();

		if (smokeIter->alpha <= 0) smokeIter = smoke.erase(smokeIter);
		else smokeIter++;
	}

	//炎更新---------------------------------------------------------
	std::list< TextureEffect >::iterator fireIter = fire.begin();
	while (fireIter != fire.end())
	{
		fireIter->update();

		if (fireIter->alpha <= 0) fireIter = fire.erase(fireIter);
		else fireIter++;
	}
}

/////////////////////////////////わ
//描画
/////////////////////////////////
void DeadEffect::draw()
{
	glPushMatrix();
	{
		glEnable(GL_ALPHA_TEST);
		glTranslatef(pos.x, pos.y, pos.z);

		//煙更新---------------------------------------------------------
		std::list< TextureEffect >::iterator smokeIter = smoke.begin();
		while (smokeIter != smoke.end())
		{
			glm::vec3 rgb(0.4, 0.4, 0.4);

			//アルファブレンド
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			smokeIter->draw(TEXTURE_ID::SMOKEEFFECT, rgb);
			smokeIter++;
		}

		//炎更新---------------------------------------------------------
		std::list< TextureEffect >::iterator fireIter = fire.begin();
		while (fireIter != fire.end())
		{
			glm::vec3 rgb(1, 0.4, 0.2);

			//アルファブレンド
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			fireIter->draw(TEXTURE_ID::SMOKEEFFECT, rgb);
			fireIter++;
		}
		glDisable(GL_ALPHA_TEST);
	}
	glPopMatrix();
}

////////////////////////////////
//削除判定
////////////////////////////////
bool DeadEffect::del()
{
	if (smoke.size() == 0 && fire.size() == 0) return true;
	
	return false;
}
