#pragma once
#include "e_setmaxhealth.h"



e_setmaxhealth::e_setmaxhealth(int _priority, float change,unsigned int _frameLimit) :Effect(_priority)
{
	frameCount = 0;
	frameLimit = _frameLimit;
	maxHP_change = change;
	isExecuted = false;
}

e_setmaxhealth::~e_setmaxhealth()
{
	if (backupCopy != NULL) {
		((e_setmaxhealth*)backupCopy)->backupCopy = NULL;
		delete (e_setmaxhealth*)backupCopy;
		backupCopy = NULL;
	}
}

void e_setmaxhealth::onStart()
{
	MortalObject* parent = dynamic_cast<MortalObject*>(GetParent());
	if (maxHP_change > 0)
	{
		parent->setMaxHealth(parent->getMaxHealth() * maxHP_change);
		isExecuted = true;
		backup();
	}
}

void e_setmaxhealth::execute()
{
	frameCount++;
	backup();
	if (frameCount >= frameLimit && isExecuted)
	{
		MortalObject* parent = dynamic_cast<MortalObject*>(GetParent());
		parent->setMaxHealth(parent->getMaxHealth() / maxHP_change);
	}
}

bool e_setmaxhealth::isEnd()
{
	if (isExecuted == false)
		return true;
	return frameCount >= frameLimit;
}
