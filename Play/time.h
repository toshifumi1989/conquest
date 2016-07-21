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
	unsigned int frame;	//Œ»İ‚ÌƒtƒŒ[ƒ€”
	int seconds;		//•b
	int minutes;		//•ª
	int rest;			//c‚èŠÔ
};

extern Time* time;

#endif