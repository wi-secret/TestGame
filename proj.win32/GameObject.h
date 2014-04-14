#pragma once
#include "cocos2d.h"
#include "Effect.h"
#include <list>
#include <algorithm>

#define STATIC_UT -1
#define DEFAULT_UT 0
#define HEAVY_UT 1
#define LIGHT_UT 2

using namespace std;
using namespace cocos2d;
class Effect;

typedef bool (*forEffect)(Effect* effect,int effectID, void* userdata);

class GameObject :
	public CCSprite
{
public:
	//effect数组的set,get和遍历
	//GameObject负责释放effect
	int addEffect(Effect* effect);
	Effect* getEffect(int effectID);
	//移除effect的时候务必使用该函数防止内存泄露
	bool removeEffect(int effectID);
	//f返回false的时候中断循环
	void foreachEffect(forEffect f,void* userdata);
	
	virtual void AI();
	
	~GameObject(void);
	
protected:
	virtual int onDestroy()=0;
	
	GameObject(void);
private:
	list<Effect*> pEffectArray;
	unsigned int maxTag;

	static bool effectPrioritySort(Effect*,Effect*);
};

class MovableObject : public GameObject {
public:
	CCPoint speed;
	double palstance;//角速度
	
	static MovableObject* create();
	static MovableObject* create(const char* image);

	void setTrueRotation(float nRotation);
	float getTrueRotation();//角度

	void setLocalSpeed(CCPoint& newSpeed);
	void addLocalSpeed(CCPoint& addSpeed);
	void setLocalSpeedX(double newSpeedX);
	void setLocalSpeedY(double newSpeedY);
	double getLocalSpeedX();
	double getLocalSpeedY();

	
	virtual void AI();

	~MovableObject();
protected:
	virtual int onDestroy();

	MovableObject();
private:
	float rotation;
};

class SelfObject: public MovableObject{
public:
	static SelfObject* create();
	static SelfObject* create(const char* image);
};

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