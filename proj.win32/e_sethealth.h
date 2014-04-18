#pragma once
#include "Effect.h"
#include "BasicObject.h"

class Effect;

class e_sethealth :public Effect{
public:
	
	virtual int getType() { return TEST_EC_HP; }
	virtual bool isType(int type) {
		if (Effect::isType(type))
			return true;
		return TEST_EC_HP == type;
	}
	e_sethealth(int _priority, int damage);
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

private:
	
	bool isExecuted;
};
