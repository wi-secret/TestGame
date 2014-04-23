#pragma once
#include "e_setenergy.h"

e_setenergy::e_setenergy(int _priority,float _change) :Effect(_priority)
{
	energy_change = _change;
	isExecuted = false;
}

e_setenergy::~e_setenergy()
{
	if (backupCopy != NULL){
		((e_setenergy*)backupCopy)->backupCopy = NULL;
		delete (e_setenergy*)backupCopy;
		backupCopy = NULL;
	}
}

void e_setenergy::onStart()
{

}

void e_setenergy::execute()
{
	EnergizedObject* parent;
	parent = dynamic_cast<EnergizedObject*>(GetParent());
	parent->setEnergyStock(parent->getEnergyStock() + energy_change);
	isExecuted = true;
}

bool e_setenergy::isEnd()
{
	return isExecuted;
}