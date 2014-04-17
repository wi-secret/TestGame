#include "e_sethealth.h"

void e_sethealth::execute()
{
	MortalObject *parent =dynamic_cast<MortalObject*>(GetParent());//需要改动
	parent->onHurt(health_change);//angle missing
	isExecuted = true;
	backup();
}

e_sethealth::e_sethealth(int _priority, int damage) :Effect(_priority)
{
	health_change = damage;
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