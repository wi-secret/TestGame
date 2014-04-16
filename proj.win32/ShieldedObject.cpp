#pragma once
#include "ShieldedObject.h"
std::map<int, cocos2d::CCAnimation *>* ShieldedObject::shieldAnimations;

ShieldedObject::ShieldedObject()
{
	MaxShield = 1;
	Shield = 1;
	ShieldType = DEFAULT_SHT;
	ShieldEfficiency = 1;
	ShieldRegenration = 0;
}
void ShieldedObject::AI()
{

}
int ShieldedObject::playShieldAnimation(int angle, int damage)
{
	CCAnimate* animate;
	map<int,cocos2d::CCAnimation*>::iterator it = getShieldAnimations()->find(0);
	if (it == getShieldAnimations()->end()) 
	{
		return 0;
	}
	else 
	{
		animate = CCAnimate::create(it->second);
	}
	animate->setDuration(1);
	runAction(CCSequence::create(animate,
		CCToggleVisibility::create(),
		NULL));
	return 1;
}

std::map<int, cocos2d::CCAnimation*>* ShieldedObject::getShieldAnimations()
{
	if (shieldAnimations == NULL)
	{
		shieldAnimations = new map<int, cocos2d::CCAnimation*>;
		for (int i = 0; i < 1; i++)
		{
			CCAnimation *animation;
			animation = CCAnimation::create();
			animation->setDelayPerUnit(0.005);
			animation->addSpriteFrameWithFileName("shield5.png");
			animation->addSpriteFrameWithFileName("shield4.png");
			animation->addSpriteFrameWithFileName("shield3.png");
			animation->addSpriteFrameWithFileName("shield2.png");
			animation->addSpriteFrameWithFileName("shield1.png");
			animation->addSpriteFrameWithFileName("shield.png");
			animation->addSpriteFrameWithFileName("shield1.png");
			animation->addSpriteFrameWithFileName("shield2.png");
			animation->addSpriteFrameWithFileName("shield3.png");
			animation->addSpriteFrameWithFileName("shield4.png");
			animation->addSpriteFrameWithFileName("shield5.png");
			animation->retain();
			(*shieldAnimations)[i] = animation;
		}
		cocos2d::CCAnimation* animation;
		animation = CCAnimation::create();
		(*shieldAnimations)[1] = animation;//WHOLE_SHIELD
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
int ShieldedObject::onDestroy()
{
	return 1;
}
ShieldedObject* ShieldedObject::create()
{
	ShieldedObject *pObject = new ShieldedObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}