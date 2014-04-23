#pragma once
#include "MovableObject.h"

#define STATIC_UT -1
#define DEFAULT_UT 0
#define HEAVY_UT 1
#define LIGHT_UT 2

class MortalObject :virtual public GameObject{
public:
	long long max_health;
	long long health;
	int healthReg;

	static MortalObject* create();
	static MortalObject* create(const char* image);

	long long setHealth(long long _health);
	long long getHealth();
	long long setMaxHealth(long long _max_health);
	long long getMaxHealth();
	int setHealthReg(int reg);
	int getHealthReg();

	virtual void AI();

	bool checkHealth();

	virtual int onHurt(int damage, int angle);//由e_health::execute()调用，播放动画。

	void cbDestroy();

	~MortalObject();
protected:

	virtual int onDestroy();//由AI调用，负责播放动画，并将自己释放

	MortalObject();

};