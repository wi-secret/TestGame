#pragma once
#include "GameObject.h"

//effect type id
#define TEST_EC_SPDLMT -1
#define TEST_EC_SPDUP -2
#define TEST_EC_HP -3
#define EC_BASE 0

//e_speeedup direction
#define E_SPDUP_DIRECT_UP 0
#define E_SPDUP_DIRECT_LEFT 1
#define E_SPDUP_DIRECT_RIGHT 2
#define E_SPDUP_DIRECT_DOWN 3

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

class e_speedlimit :public Effect {
public:

	virtual int getType() { return TEST_EC_SPDLMT; }
	virtual bool  isType(int type) {
		if(Effect::isType(type))
			return true;
		return type==TEST_EC_SPDLMT;
	} 
	e_speedlimit(int _priority):Effect(_priority){ }
	virtual ~e_speedlimit();
protected:
	virtual void backup() {
		if(backupCopy==NULL) {
			backupCopy=new e_speedlimit(*this);
			((e_speedlimit*)backupCopy)->backupCopy=backupCopy;
		} else {
			*((e_speedlimit*)backupCopy)=*this;
		}
	}
	virtual void recovery() {
		if(backupCopy!=NULL) {
			*this=*((e_speedlimit*)backupCopy);
			Effect::recovery();
		}
	}

	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();
};

class e_speedup : public Effect {
public:
	virtual int getType() { return TEST_EC_SPDUP; }
	virtual bool isType(int type) {
		if(Effect::isType(type))
			return true;
		return TEST_EC_SPDUP==type;
	}

	e_speedup(int _priority,int direct);
	virtual ~e_speedup();
protected:
	virtual void backup() {
		if(backupCopy==NULL) {
			backupCopy=new e_speedup(*this);
			((e_speedup*)backupCopy)->backupCopy=backupCopy;
		} else {
			*((e_speedup*)backupCopy)=*this;
		}
	}
	virtual void recovery() {
		*this=*((e_speedup*)backupCopy);
		Effect::recovery();
	}

	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();

	//前进方向
	int direction;
	//是否已经作用过
	bool activated;
	//遍历effectArray的函数
	static bool killLimit(Effect* effect,int effectID, void* userdata);
};

class e_sethealth:public Effect{
public:	
	virtual int getType() { return TEST_EC_HP; }
	virtual bool isType(int type) {
		if (Effect::isType(type))
			return true;
		return TEST_EC_HP == type;
	}
	e_sethealth(int _priority,int damage);
	virtual ~e_sethealth();
protected:
	virtual void backup() {
		if (backupCopy == NULL) {
			backupCopy = new e_sethealth(*this);
			((e_sethealth*)backupCopy)->backupCopy = backupCopy;
		}
		else {
			*((e_sethealth*)backupCopy) = *this;
		}
	}
	virtual void recovery() {
		*this = *((e_sethealth*)backupCopy);
		Effect::recovery();
	}
	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();

	int health_change;


};
