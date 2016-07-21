#ifndef TIME_H
#define TIME_H


class Time
{
public:
	Time() :
		frame(0),
		rest(3600),
		seconds(0)
	{	
		minutes = rest / 3600;
	}
	~Time() {}

	void update();
	void draw();
	int getMinutes();

private:
	unsigned int frame;	//���݂̃t���[����
	int seconds;		//�b
	int minutes;		//��
	int rest;			//�c�莞��
};

extern Time* time;

#endif