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


