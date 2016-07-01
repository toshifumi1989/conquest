#include "pole.h"
#include "../glut.h"
#include "player.h"

std::vector< Pole* >pole;

//////////////////////////////////////////
//
//////////////////////////////////////////
void Pole::update()
{
	const float recovery = 0.1;
	if (onPlayer)
	{
		//�v���C���[���̂Ƃ��̎�����
		//�v���C���[���̂Ƃ��̍ő�l��maxHP
		if (HP < maxHP)	//HP��max�ɂȂ��Ă��Ȃ��Ȃ�
		{
			HP += recovery;
			if (HP >= maxHP)	//HP��max�𒴂����Ƃ�
			{
				HP = maxHP;		//HP��max�ɂ���
			}
		}
	}
	else
	{
		//�G�l�~�[���̂Ƃ��̎�����
		//�G�l�~�[���̂Ƃ��̍ő�l��0
		if (HP > 0)	//HP��0���傫���Ƃ�
		{
			HP -= recovery;
			if (HP <= 0)		//HP��0�𒴂����Ƃ�
			{
				HP = 0;			//HP��0�ɂ���
			}
		}
	}
}

///////////////////////////////////////////
//�`��
////////////////////////////////////////////
void Pole::draw()
{
	glPushMatrix();
	{
		glEnable(GL_DEPTH_TEST);


		//�~��---------------------------------------------------
		if (onPlayer)
		{
			color = player->color;
		}
		else
		{
			color = glm::vec3(1, 0, 0);
		}

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y, pos.z);


		glRotatef(-90, 1, 0, 0);//�����𗧂Ă�
		GLUquadricObj *quad = gluNewQuadric();//quadric object ����������� 
		gluCylinder(quad, radius, radius, height, sides, 1);//�~��


		//HP�o�[---------------------------------------------------
		glRotatef(90, 1, 0, 0);//�~���̌�����߂�
		const float height = 1;				//HP�o�[�̍���
		const float playerPole = HP / 20.f;	//player���̕���
		const float maxShow = maxHP / 20.f;	//HP�o�[�̉��̒���

		const float posHeight = 5.0f;
		glTranslatef(-maxShow / 2, posHeight, 0);
		glBegin(GL_QUADS);
		{
			//�v���C���[����HP
			glColor3f(player->color.r, player->color.g, player->color.b);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(playerPole, height, 0);
			glVertex3f(playerPole, 0, 0);

			//�G�l�~�[����HP
			glColor3f(1, 0, 0);
			glVertex3f(playerPole, 0, 0);
			glVertex3f(playerPole, 0, 0);
			glVertex3f(maxShow, 0, 0);
			glVertex3f(maxShow, 0, 0);
		}
		glEnd();


		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();

}