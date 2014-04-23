#pragma once
#include "EnergizedObject.h"

class e_setenergy: public Effect
{
public:
	
	virtual int getType() { return TEST_EC_ENERGY; }
	virtual bool isType(int type) {
		if (Effect::isType(type))
			return true;
		return TEST_EC_ENERGY == type;
	}

	e_setenergy(int _priority,float change);

	virtual ~e_setenergy();

protected:

	virtual void backup() {
		if (backupCopy == NULL) {
			backupCopy = new e_setenergy(*this);
			((e_setenergy*)backupCopy)->backupCopy = backupCopy;
		}
		else {
			*((e_setenergy*)backupCopy) = *this;
		}
	}
	virtual void recovery() {
		*this = *((e_setenergy*)backupCopy);
		Effect::recovery();
	}

	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();
private:

	float energy_change;

	bool isExecuted;
};

