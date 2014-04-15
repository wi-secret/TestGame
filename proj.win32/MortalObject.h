#pragma once
#include "MovableObject.h"

#define STATIC_UT -1
#define DEFAULT_UT 0
#define HEAVY_UT 1
#define LIGHT_UT 2

class MortalObject :public MovableObject{
public:
	long long max_health;
	long long health;
	int unit_type;//生命类实例的种类。牵涉到具体游戏内容，影响AI()执行方式
	int level;//等级。用于决定生命上限，以及速度上限(未实现)

	static MortalObject* create();
	static MortalObject* create(const char* image);
	static MortalObject* create(const char* image, int level);
	static MortalObject* create(const char* image, int level, int unit_type);

	virtual void AI();

	bool check();

	virtual int onHurt();//由e_health::execute()调用，播放动画。

	int isDestroyed;

	void cbDestroy();

	~MortalObject();
protected:

	virtual int onDestroy();//由AI调用，负责播放动画，并将自己释放

	MortalObject();

};