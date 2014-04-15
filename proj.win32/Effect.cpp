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

