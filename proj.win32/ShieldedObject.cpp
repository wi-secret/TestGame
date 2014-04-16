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

int ShieldedObject::playShieldAnimation(int angle, int damage)
{
	CCAnimate* animate;
	map<int,cocos2d::CCAnimation*>::iterator it = getShieldAnimations()->find(angle);

	if (it == getShieldAnimations()->end()) 
	{
		return 0;
	}
	else 
	{
		animate = CCAnimate::create(it->second);
	}
	runAction(animate);
	return 1;
}

std::map<int, cocos2d::CCAnimation*>* ShieldedObject::getShieldAnimations()
{
	if (shieldAnimations == NULL)
	{
		for (int i = 0; i < 360; i++)
		{
			cocos2d::CCAnimation *animation;
			animation = CCAnimation::create();
			(*shieldAnimations)[i] = animation;
		}
		cocos2d::CCAnimation* animation;
		animation = CCAnimation::create();
		(*shieldAnimations)[WHOLE_SHIELD] = animation;
	}
	return shieldAnimations;
}

long long ShieldedObject::SetMaxShield(long long _maxshield)
{
	MaxShield = _maxshield;
	return MaxShield;
}
long long ShieldedObject::GetMaxShield()
{
	return MaxShield;
}

int ShieldedObject::GetShieldReg()
{
	return ShieldRegenration;
}
int ShieldedObject::SetShieldReg(int _shieldreg)
{
	ShieldRegenration = _shieldreg;
	return ShieldRegenration;
}

long long ShieldedObject::SetShield(long long _shield)
{
	Shield = _shield;
	return Shield;
}
long long ShieldedObject::GetShield()
{
	return Shield;
}
ShieldedObject::~ShieldedObject()
{

}