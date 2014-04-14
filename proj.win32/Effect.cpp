#include "Effect.h"


Effect::Effect(int _priority)
{
	priority = _priority;
	backupCopy = NULL;
	typeID = EC_BASE;
	__execute =&Effect::execute;
	__onStart = &Effect::onStart;
	__isEnd = &Effect::isEnd;
}


Effect::~Effect(void)
{
	if(backupCopy!=NULL) {
		delete backupCopy;
	}
}

GameObject* Effect::GetParent() {
	return parent;
}


void e_speedlimit::execute() {
	MovableObject *par = (MovableObject*)GetParent();
	par->speed = ccp(0, 0);
}

void e_speedlimit::onStart() {
}

bool e_speedlimit::isEnd() {
	return false;
}

e_speedlimit::~e_speedlimit() {
	if (backupCopy != NULL) {
		((e_speedlimit*)backupCopy)->backupCopy = NULL;
		delete (e_speedlimit*)backupCopy;
		backupCopy = NULL;
	}
}

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
	MovableObject *par=(MovableObject*)GetParent();
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
	par->foreachEffect(killLimit,this);
	activated=true;
	backup();
}

void e_speedup::onStart() {
	activated=false;
}

bool e_speedup::isEnd() {
	return activated;
}

bool e_speedup::killLimit(Effect* effect,int effectID, void* userdata) {
	e_speedup *_this=(e_speedup*)userdata;
	if(effect->isType(TEST_EC_SPDLMT)) {
		effect->__execute=&Effect::exeEmpty;
	}
	return true;
}

void e_sethealth::execute(){
	MortalObject *parent = (MortalObject*)GetParent();
	parent->health += this->health_change;
	parent->onHurt();
}

e_sethealth::e_sethealth(int _priority,int damage) :Effect(_priority)
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