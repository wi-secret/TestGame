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

	long long SetHealth(long long _health);
	long long GetHealth();
	long long SetMaxHealth(long long _max_health);
	long long GetMaxHealth();
	int SetHealthReg(int reg);
	int GetHealthReg();

	virtual void AI();

	bool checkHealth();

	virtual int onHurt(int damage);//由e_health::execute()调用，播放动画。

	int isDestroyed;

	virtual void cbDestroy();

	~MortalObject();
protected:

	virtual int onDestroy();//由AI调用，负责播放动画，并将自己释放

	MortalObject();

};