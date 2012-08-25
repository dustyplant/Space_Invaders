#ifndef TIMER_H
#define TIMER_H

class Timer{
private:
	int startTicks;

public:
	Timer();
	void start();
	int get_ticks();
};

#endif