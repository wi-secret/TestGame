#pragma once
#include "Effect.h"
#include "MortalObject.h"
/*
* e_setmaxhealth������������������
* change=1.0Ϊ���޲���,change�����0
* frameCountΪeffect����֡��
*/
class e_setmaxhealth :public Effect
{
public:

	virtual int getType() { return TEST_EC_MAXHP; }
	virtual bool isType(int type) {
		if (Effect::isType(type))
			return true;
		return TEST_EC_MAXHP == type;
	}

	e_setmaxhealth(int _priority, float change,unsigned int frameCount);

	virtual ~e_setmaxhealth();

protected:

	virtual void backup() {
		if (backupCopy == NULL) {
			backupCopy = new e_setmaxhealth(*this);
			((e_setmaxhealth*)backupCopy)->backupCopy = backupCopy;
		}
		else {
			*((e_setmaxhealth*)backupCopy) = *this;
		}
	}
	virtual void recovery() {
		*this = *((e_setmaxhealth*)backupCopy);
		Effect::recovery();
	}

	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();
private:

	float maxHP_change;//1.0Ϊ���䣬����0.0��2.0

	bool isExecuted;

	unsigned int frameCount;
	unsigned int frameLimit;
};
