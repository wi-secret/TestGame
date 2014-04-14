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
//�κ�������Effect������Ҫʵ�����º���
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
	
	//���ֺ�����д�ɺ�
	bool virtual isType(int type) { return type==EC_BASE; }
	
	//����3��������������GameObject����
	void (Effect::*__execute)();
	void (Effect::*__onStart)();
	bool (Effect::*__isEnd)();

	//����һЩԤ�����execute����
	void exeEmpty() {}

	//����һЩԤ�����isEnd����
	bool ieEternal() { return false; }
	bool ieInstant() { return true; }

protected:
	//ͨ������,����������������������ָ��һһ��Ӧ,�����ǵ��е�effectҲӵ���޸���3�������
	//��Ҫ�޸����������ݱ����ڶ�Ӧ���������Ӧ�ĳ�Ա����,���Բ��Ƽ������޸ķ�ʽ
	virtual void execute()=0;
	virtual void onStart()=0;
	virtual bool isEnd()=0;
	
	//���������ͷ�backupCopy
	virtual ~Effect();

	GameObject* GetParent();
	
	//���ֺ�����д�ɺ�
	//GameObject::addEffect()�Զ�Ϊeffect����һ�α���
	//��GameObject::AI()��,������effect��ִ�к󽫽���һ�λָ�
	virtual void backup()=0;
	virtual void recovery() {
		if(backupCopy!=NULL)
			*this=*backupCopy;
	}

	Effect* backupCopy;
	int typeID;
private:
	//��GameObject����tag,parent��ֵ,�����ɵ�ʱ�̾���priority,���;���typeID
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

	//ǰ������
	int direction;
	//�Ƿ��Ѿ����ù�
	bool activated;
	//����effectArray�ĺ���
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
