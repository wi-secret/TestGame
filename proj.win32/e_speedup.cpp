#include "e_speedup.h"

e_speedup::~e_speedup() {
	if (backupCopy != NULL) {
		((e_speedup*)backupCopy)->backupCopy = NULL;
		delete (e_speedup*)backupCopy;
		backupCopy = NULL;
	}
}

e_speedup::e_speedup(int _priority, int direct) :Effect(_priority) {
	direction = direct;
}

void e_speedup::execute() {
	MovableObject *par = dynamic_cast<MovableObject*>(GetParent());
	switch (direction)
	{
	case E_SPDUP_DIRECT_DOWN:
		par->setLocalSpeedY(-1);
		break;
	case E_SPDUP_DIRECT_LEFT:
		par->setLocalSpeedX(-1);
		break;
	case E_SPDUP_DIRECT_RIGHT:
		par->setLocalSpeedX(1);
		break;
	case E_SPDUP_DIRECT_UP:
		par->setLocalSpeedY(1);
		break;
	default:
		break;
	}
	par->foreachEffect(killLimit, this);
	activated = true;
	backup();
}

void e_speedup::onStart() {
	activated = false;
}

bool e_speedup::isEnd() {
	return activated;
}

bool e_speedup::killLimit(Effect* effect, int effectID, void* userdata) {
	e_speedup *_this = (e_speedup*)userdata;
	if (effect->isType(TEST_EC_SPDLMT)) {
		effect->__execute = &Effect::exeEmpty;
	}
	return true;
}