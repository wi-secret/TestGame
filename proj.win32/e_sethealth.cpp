#include "e_sethealth.h"

void e_sethealth::execute()
{
	MortalObject *parent = dynamic_cast<MortalObject*>(GetParent());//不一般，需要改动MortalObject
	parent->onHurt(health_change, angle);
	isExecuted = true;
	backup();
}

e_sethealth::e_sethealth(int _priority, int _damage,int _angle) :Effect(_priority)
{
	angle = _angle;
	health_change = _damage;
	isExecuted = false;
}

e_sethealth::~e_sethealth() {
	if (backupCopy != NULL) {
		((e_sethealth*)backupCopy)->backupCopy = NULL;
		delete (e_sethealth*)backupCopy;
		backupCopy = NULL;
	}
}

void e_sethealth::onStart()
{
}

bool e_sethealth::isEnd()
{
	return isExecuted;
}