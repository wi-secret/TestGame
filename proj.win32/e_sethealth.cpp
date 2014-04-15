#include "e_sethealth.h"

void e_sethealth::execute(){
	MortalObject *parent = (MortalObject*)GetParent();
	parent->health += this->health_change;
	parent->onHurt();
}

e_sethealth::e_sethealth(int _priority, int damage) :Effect(_priority)
{
	health_change = damage;
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
	MortalObject *parent = (MortalObject*)GetParent();
	parent->health += this->health_change;
	parent->onHurt();
}
bool e_sethealth::isEnd()
{
	if (((MortalObject*)GetParent())->isDestroyed)
		return 1;
	else return 0;
}