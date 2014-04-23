#pragma once
#include "cocos2d.h"
#include "Effect.h"
#include <list>
#include <algorithm>

using namespace std;
using namespace cocos2d;
class Effect;

#define TEST_GAME_OBJ -1
#define TEST_MOV_OBJ -2
#define TEST_MOR_OBJ -3
#define TEST_ENERGY_OBJ -4
#define TEST_SHIELD_OBJ -5
#define TEST_BASIC_OBJ -6

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
	
	bool isDestroyed;
protected:
	virtual int onDestroy()=0;
	
	int ObjectType;

	GameObject(void);
private:
	list<Effect*> pEffectArray;
	unsigned int maxTag;


	static bool effectPrioritySort(Effect*,Effect*);
};

