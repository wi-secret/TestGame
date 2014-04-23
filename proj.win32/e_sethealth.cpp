#include "e_sethealth.h"

void e_sethealth::execute()
{
	MortalObject *parent = dynamic_cast<MortalObject*>(GetParent());//不一般，需要改动MortalObject
	if (health_change < 0)
	{
		parent->onHurt(health_change, angle);
	}
	if (health_change > 0)
	{
		parent->setHealth(parent->getHealth() + health_change);
	}
	isExecuted = true;
	backup();
}

e_sethealth::e_sethealth(int _priority, int HP_change,int _angle) :Effect(_priority)
{
	angle = _angle;
	health_change = HP_change;
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