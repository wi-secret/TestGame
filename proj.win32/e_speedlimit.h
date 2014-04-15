#pragma once
#include "Effect.h"
#include "MovableObject.h"

class Effect;

class e_speedlimit :public Effect {
public:

	virtual int getType() { return TEST_EC_SPDLMT; }
	virtual bool  isType(int type) {
		if (Effect::isType(type))
			return true;
		return type == TEST_EC_SPDLMT;
	}
	e_speedlimit(int _priority) :Effect(_priority){ }
	virtual ~e_speedlimit();
protected:
	virtual void backup() {
		if (backupCopy == NULL) {
			backupCopy = new e_speedlimit(*this);
			((e_speedlimit*)backupCopy)->backupCopy = backupCopy;
		}
		else {
			*((e_speedlimit*)backupCopy) = *this;
		}
	}
	virtual void recovery() {
		if (backupCopy != NULL) {
			*this = *((e_speedlimit*)backupCopy);
			Effect::recovery();
		}
	}

	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();
};