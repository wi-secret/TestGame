#pragma once
#include "Effect.h"
#include "MovableObject.h"

//e_speeedup direction
#define E_SPDUP_DIRECT_UP 0
#define E_SPDUP_DIRECT_LEFT 1
#define E_SPDUP_DIRECT_RIGHT 2
#define E_SPDUP_DIRECT_DOWN 3


class e_speedup : public Effect {
public:
	virtual int getType() { return TEST_EC_SPDUP; }
	virtual bool isType(int type) {
		if (Effect::isType(type))
			return true;
		return TEST_EC_SPDUP == type;
	}

	e_speedup(int _priority, int direct);
	virtual ~e_speedup();
protected:
	virtual void backup() {
		if (backupCopy == NULL) {
			backupCopy = new e_speedup(*this);
			((e_speedup*)backupCopy)->backupCopy = backupCopy;
		}
		else {
			*((e_speedup*)backupCopy) = *this;
		}
	}
	virtual void recovery() {
		*this = *((e_speedup*)backupCopy);
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
	static bool killLimit(Effect* effect, int effectID, void* userdata);
};