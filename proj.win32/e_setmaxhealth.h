#pragma once
#include "Effect.h"
#include "MortalObject.h"
/*
* e_setmaxhealth按比例增减生命上限
* change=1.0为上限不变,change须大于0
* frameCount为effect持续帧数
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

	float maxHP_change;//1.0为不变，建议0.0到2.0

	bool isExecuted;

	unsigned int frameCount;
	unsigned int frameLimit;
};
