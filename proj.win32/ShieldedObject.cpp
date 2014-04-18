#pragma once
#include "ShieldedObject.h"
#include "e_sethealth.h"
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
	Shield += ShieldRegenration;
	ShieldEfficiency = Shield / MaxShield;
	checkShield();
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
	animate->setDuration(0.3f);
	//runAction(animate);
	this->addChild(node);
	node->setRotation(angle);
	node->runAction(animate);
	return 1;
}

int ShieldedObject::onHurt(int change,int angle)
{
	if (Shield + change * ShieldEfficiency >= 0)
	{
		Shield += change * ShieldEfficiency;
		addEffect(new e_sethealth(0, change*(1 - ShieldEfficiency)));
	}
	else
	{
		change += Shield;
		Shield = 0;
		addEffect(new e_sethealth(0, change));
	}
	playShieldAnimation(change, angle);
	return Shield;
}

bool ShieldedObject::checkShield()
{
	if (Shield > MaxShield)
	{
		Shield = MaxShield;
	}
	if (ShieldEfficiency > 1)
	{
		ShieldEfficiency = 1;
	}
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
			animation->setDelayPerUnit(0.05f);
			animation->addSpriteFrameWithFileName("shield7.png");
			animation->addSpriteFrameWithFileName("shield6.png");
			animation->addSpriteFrameWithFileName("shield5.png");
			animation->addSpriteFrameWithFileName("shield4.png");
			animation->addSpriteFrameWithFileName("shield3.png");
			animation->addSpriteFrameWithFileName("shield2.png");
			animation->addSpriteFrameWithFileName("shield1.png");
			animation->addSpriteFrameWithFileName("shield.png");
			animation->addSpriteFrameWithFileName("shield.png");
			animation->addSpriteFrameWithFileName("shield.png");
			animation->addSpriteFrameWithFileName("shield1.png");
			animation->addSpriteFrameWithFileName("shield2.png");
			animation->addSpriteFrameWithFileName("shield3.png");
			animation->addSpriteFrameWithFileName("shield4.png");
			animation->addSpriteFrameWithFileName("shield5.png");
			animation->addSpriteFrameWithFileName("shield6.png");
			animation->addSpriteFrameWithFileName("shield7.png");
			animation->addSpriteFrameWithFileName("shield0.png");
			(*shieldAnimations)[i] = animation;
		}
		cocos2d::CCAnimation* animation;
		animation = CCAnimation::create();
		(*shieldAnimations)[1] = animation;//WHOLE_SHIELD
	}
	return shieldAnimations;
}

long long ShieldedObject::SetMaxShield(long long in_maxshield)
{
	MaxShield = in_maxshield;
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
int ShieldedObject::SetShieldReg(int in_shieldReg)
{
	ShieldRegenration = in_shieldReg;
	return ShieldRegenration;
}

long long ShieldedObject::SetShield(long long in_shield)
{
	Shield = in_shield;
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
	pObject->node = new CCNode();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		if (pObject->node && pObject->node->init())
		{
			//pObject->node->setPosition(ccp(0, 0));
			//pObject->addChild(pObject->node);
		}
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

ShieldedObject* ShieldedObject::create(const char* image)
{
	ShieldedObject *pObject = new ShieldedObject();
	pObject->node = new CCNode();
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		if (pObject->node && pObject->node->init())
		{
			//pObject->addChild(pObject->node);
		}
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}