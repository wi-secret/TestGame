#pragma once
#include "ShieldedObject.h"

ShieldedObject::ShieldedObject()
{
	MaxShield = 1;
	Shield = 1;
	ShieldType = DEFAULT_SHT;
	ShieldEfficiency = 1;
	ShieldRegenration = 0;
}
/*
std::map<int, cocos2d::CCAnimation>* ShieldedObject::getShieldAnimations()
{
	return shieldAnimations;
}
*/
int ShieldedObject::SetMaxShield()
{
	return MaxShield;
}
int ShieldedObject::GetMaxShield()
{
	return MaxShield;
}

int ShieldedObject::GetShieldReg()
{
	return ShieldRegenration;
}
int ShieldedObject::SetShieldReg()
{
	return ShieldRegenration;
}

int ShieldedObject::SetShield()
{
	return Shield;
}
int ShieldedObject::GetShield()
{
	return Shield;
}
ShieldedObject::~ShieldedObject()
{

}