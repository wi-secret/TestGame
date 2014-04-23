#pragma once
#include "Effect.h"

//时限影响，设定存在时限，用于生命周期限定

class e_timelimit : public Effect
{
public:
	virtual int getType() { return TEST_EC_TMLMT; }
	virtual bool isType(int type) {
		if (Effect::isType(type))
			return true;
		return TEST_EC_TMLMT == type;
	}

	e_timelimit(int _priority, unsigned int frameLimit);

	virtual ~e_timelimit();

protected:

	virtual void backup() {
		if (backupCopy == NULL) {
			backupCopy = new e_timelimit(*this);
			((e_timelimit*)backupCopy)->backupCopy = backupCopy;
		}
		else {
			*((e_timelimit*)backupCopy) = *this;
		}
	}
	virtual void recovery() {
		*this = *((e_timelimit*)backupCopy);
		Effect::recovery();
	}

	virtual void execute();
	virtual void onStart();
	virtual bool isEnd();


private:
	unsigned int frameCount;

	unsigned int frameLimit;

};