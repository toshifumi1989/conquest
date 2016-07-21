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
	unsigned int frame;	//現在のフレーム数
	int seconds;		//秒
	int minutes;		//分
	int rest;			//残り時間
};

extern Time* time;

#endif