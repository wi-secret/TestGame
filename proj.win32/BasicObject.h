#pragma once
#include "MortalObject.h"

class BasicObject : public MortalObject
{
public:
	int unit_type;//生命类实例的种类。牵涉到具体游戏内容，影响AI()执行方式
	int level;//等级。用于决定生命上限，以及速度上限(未实现)

	static BasicObject* create(const char* image, int level);
	static BasicObject* create(const char* image, int level, int unit_type);

	virtual void AI();

	virtual int onHurt();//由e_health::execute()调用，播放动画。

	~BasicObject();

protected:
	BasicObject();

	virtual int onDestroy();//由AI调用，负责播放动画，并将自己释放
};