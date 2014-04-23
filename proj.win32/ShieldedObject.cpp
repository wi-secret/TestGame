#pragma once
#include "ShieldedObject.h"
#include "e_sethealth.h"
std::map<int, cocos2d::CCAnimation *>* ShieldedObject::shieldAnimations;

ShieldedObject::ShieldedObject()
{
	ObjectType = TEST_SHIELD_OBJ;
	MaxShield = 1;
	Shield = 1;
	ShieldType = DEFAULT_SHT;
	ShieldEfficiency = 1;
	ShieldRegenration = 1;

}

void ShieldedObject::AI()
{
	GameObject::AI();//ShieldObject不用于实例化对象，则此行可去除
	Shield += ShieldRegenration;
	//ShieldEfficiency = Shield / MaxShield;
	checkShield();
}

int ShieldedObject::playShieldAnimation(int damage, int angle)
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
	animate->setDuration(0.13f);
	//runAction(animate);
	node->setAnchorPoint(ccp(0.5, 0.5));
	node->setRotation(angle);
	node->runAction(animate);
	return 1;
}

int ShieldedObject::onHurt(int change,int angle)
{
	playShieldAnimation(change, angle);
	if (Shield + change * ShieldEfficiency >= 0)
	{
		Shield += change * ShieldEfficiency;
		if (change*(1-ShieldEfficiency) < 0)
		addEffect(new e_sethealth(-1, change*(1 - ShieldEfficiency),angle));
	}
	else
	{
		change += Shield;
		Shield = 0;
		addEffect(new e_sethealth(-1, change, angle));
	}
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
			animation->retain();
			animation->setDelayPerUnit(0.05f);
			animation->addSpriteFrameWithFileName("res/image/shield7.png");
			animation->addSpriteFrameWithFileName("res/image/shield6.png");
			animation->addSpriteFrameWithFileName("res/image/shield5.png");
			animation->addSpriteFrameWithFileName("res/image/shield4.png");
			animation->addSpriteFrameWithFileName("res/image/shield3.png");
			animation->addSpriteFrameWithFileName("res/image/shield2.png");
			animation->addSpriteFrameWithFileName("res/image/shield1.png");
			animation->addSpriteFrameWithFileName("res/image/shield.png");
			animation->addSpriteFrameWithFileName("res/image/shield.png");
			animation->addSpriteFrameWithFileName("res/image/shield.png");
			animation->addSpriteFrameWithFileName("res/image/shield1.png");
			animation->addSpriteFrameWithFileName("res/image/shield2.png");
			animation->addSpriteFrameWithFileName("res/image/shield3.png");
			animation->addSpriteFrameWithFileName("res/image/shield4.png");
			animation->addSpriteFrameWithFileName("res/image/shield5.png");
			animation->addSpriteFrameWithFileName("res/image/shield6.png");
			animation->addSpriteFrameWithFileName("res/image/shield7.png");
			animation->addSpriteFrameWithFileName("res/image/shield0.png");
			(*shieldAnimations)[i] = animation;
		}
	}
	return shieldAnimations;
}

long long ShieldedObject::setMaxShield(long long in_maxshield)
{
	MaxShield = in_maxshield;
	return MaxShield;
}
long long ShieldedObject::getMaxShield()
{
	return MaxShield;
}

int ShieldedObject::getShieldReg()
{
	return ShieldRegenration;
}
int ShieldedObject::setShieldReg(int in_shieldReg)
{
	ShieldRegenration = in_shieldReg;
	return ShieldRegenration;
}

long long ShieldedObject::setShield(long long in_shield)
{
	Shield = in_shield;
	return Shield;
}
long long ShieldedObject::getShield()
{
	return Shield;
}
ShieldedObject::~ShieldedObject()
{
	for (map<int, cocos2d::CCAnimation*>::iterator it = getShieldAnimations()->begin();
		it != getShieldAnimations()->end();
		it++)
	if (it->second != NULL)
	{
		delete (it->second);
		it->second = NULL;
	}
}
int ShieldedObject::onDestroy()
{
	return 1;
}
ShieldedObject* ShieldedObject::create()
{
	ShieldedObject *pObject = new ShieldedObject();
	pObject->node = new CCSprite();
	if (pObject && pObject->init())
	{
		pObject->node->init();
		pObject->addChild(pObject->node);
		pObject->node->autorelease();
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

ShieldedObject* ShieldedObject::create(const char* image)
{
	ShieldedObject *pObject = new ShieldedObject();
	pObject->node = new CCSprite();
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		if (pObject->node && pObject->node->init())
		{
			pObject->node->init();
			pObject->node->setPosition(ccp(pObject->getContentSize().width / 2,
				pObject->getContentSize().height/ 2));
			pObject->addChild(pObject->node);
			pObject->node->autorelease();
			pObject->autorelease();
		}
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}