#pragma once
#include "GameObject.h"
//#include "MortalObject.h"
//#include "MovableObject.h"
//#include "SelfObject.h"
//effect type id

#define EC_BASE 0
#define TEST_EC_SPDLMT -1
#define TEST_EC_SPDUP -2
#define TEST_EC_HP -3

class GameObject;
//任何派生自Effect的类需要实现如下函数
/*
public:
virtual int getType();
virtual bool isType(int type);

virtual ~Effect();

protected:
virtual void execute();
virtual void onStart();
virutal bool isEnd();

virtual void backup();
virtual void recovery();

*/
class Effect
{
public:
	Effect(int _priority);
	virtual int getType() { return EC_BASE; }
	
	int priority;
	
	//这种函数请写成宏
	bool virtual isType(int type) { return type==EC_BASE; }
	
	//以下3个函数均由宿主GameObject调用
	void (Effect::*__execute)();
	void (Effect::*__onStart)();
	bool (Effect::*__isEnd)();

	//定义一些预定义的execute函数
	void exeEmpty() {}

	//定义一些预定义的isEnd函数
	bool ieEternal() { return false; }
	bool ieInstant() { return true; }

protected:
	//通常情形,下面三个函数与上面三个指针一一对应,但考虑到有的effect也拥有修改这3项的能力
	//想要修改这三项内容必须在对应类中添加相应的成员函数,所以不推荐这种修改方式
	virtual void execute()=0;
	virtual void onStart()=0;
	virtual bool isEnd()=0;
	
	//根据类型释放backupCopy
	virtual ~Effect();

	GameObject* GetParent();
	
	//这种函数请写成宏
	//GameObject::addEffect()自动为effect进行一次备份
	//在GameObject::AI()中,当所有effect被执行后将进行一次恢复
	virtual void backup()=0;
	virtual void recovery() {
		if(backupCopy!=NULL)
			*this=*backupCopy;
	}

	Effect* backupCopy;
	int typeID;
private:
	//由GameObject决定tag,parent的值,在生成的时刻决定priority,类型决定typeID
	int tag;
	GameObject *parent;

	friend class GameObject;
};


