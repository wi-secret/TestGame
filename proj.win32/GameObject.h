#pragma once
#include "cocos2d.h"
#include "Effect.h"
#include <list>
#include <algorithm>

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

