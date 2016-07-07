#include "pole.h"
#include "../glut.h"
#include "player.h"

std::vector< Pole* >pole;

//////////////////////////////////////////
//������
//////////////////////////////////////////
void Pole::recovery()
{
	//���w�̂Ƃ�������
	const int recovery = 1;
	if (type == TYPE::BLUE)
	{
		//�v���C���[���̂Ƃ��̎�����
		//�v���C���[���̂Ƃ��̍ő�l��maxHP
		if (HP < maxHP)	//HP��max�ɂȂ��Ă��Ȃ��Ȃ�
		{
			HP += recovery;

		}
		//HP��max�𒴂����Ƃ�HP��max�ɂ���
		if (HP >= maxHP)
		{
			HP = maxHP;
		}

	}
	else if (type == TYPE::RED)
	{
		//�G�l�~�[���̂Ƃ��̎�����
		//�G�l�~�[���̂Ƃ��̍ő�l��0
		if (HP > 0)	//HP��0���傫���Ƃ�
		{
			HP -= recovery;
		}
		//HP��0�𒴂����Ƃ�HP��0�ɂ���
		if (HP <= 0)
		{
			HP = 0;
		}
	}

}


///////////////////////////////////////////
//���  (�^�C�v�̐؂�ւ�
///////////////////////////////////////////
void Pole::occupation()
{
	if (HP >= maxHP)
	{
		type = TYPE::BLUE;

	}


	if (HP <= 0)
	{
		type = TYPE::RED;

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
		const glm::vec3 blueColor(0.2f, 0.4f, 0.7f);	//BLUE�w�c�̐F
		const glm::vec3 redColor(0.7f, 0.2f, 0.1f);		//RED�w�c�̐F

		//�~��---------------------------------------------------
		if (type == TYPE::BLUE)
		{
			color = blueColor;
		}
		else if (type == TYPE::RED)
		{
			color = redColor;
		}
		else
		{
			color = glm::vec3(1, 1, 1);
		}

		glColor3f(color.r, color.g, color.b);
		glTranslatef(pos.x, pos.y, pos.z);

		glPushMatrix();
		glRotatef(-90, 1, 0, 0);							//�����𗧂Ă�
		GLUquadricObj *quad = gluNewQuadric();				//quadric object ����������� 
		gluCylinder(quad, radius, radius, height, sides, 1);//�~��
		glPopMatrix();

		//�r���{�[�h�s��ݒ�--------
		glm::mat4 view;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);

		glm::mat4 m = glm::inverse(view);
		m[3][0] = m[3][1] = m[3][2] = 0;
		glMultMatrixf((float*)&m);
		//--------------------------
		//HP�o�[---------------------------------------------------

		const float height = 1;						//HP�o�[�̍���
		const float playerPole = HP / 50.f;			//player���̕���
		const float maxShow = maxHP / 50.f;			//HP�o�[�̉��̒���
		const float posHeight = 10.0f;				//HP�o�[�̍���

		glTranslatef(-maxShow / 2, posHeight, 0);
		glBegin(GL_QUADS);
		{
			//�v���C���[����HP
			glColor3f(blueColor.r, blueColor.g, blueColor.b);
			glVertex3f(0, 0, 0);
			glVertex3f(0, height, 0);
			glVertex3f(playerPole, height, 0);
			glVertex3f(playerPole, 0, 0);

			//�G�l�~�[����HP
			glColor3f(redColor.r, redColor.g, redColor.b);
			glVertex3f(playerPole, 0, 0);
			glVertex3f(playerPole, height, 0);
			glVertex3f(maxShow, height, 0);
			glVertex3f(maxShow, 0, 0);
		}
		glEnd();


		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();

}