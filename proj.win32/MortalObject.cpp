#pragma once
#include "MortalObject.h"
#include "GameScene.h"

MortalObject::MortalObject()
{
	ObjectType = TEST_MOR_OBJ;
	max_health = 1;
	health = 1;
	isDestroyed = 0;
	healthReg = 0;
}

MortalObject::~MortalObject()
{
}

void MortalObject::cbDestroy()
{
	this->getParent()->removeChild(this, true);
}

int MortalObject::onDestroy()
{
	
	CCBlink* action;
	action = CCBlink::create(1, 3);
	runAction(CCSequence::create( action,
		CCCallFunc::create(this, callfunc_selector(CCSprite::removeFromParent)),
		NULL));
	return 1;
}

void MortalObject::AI()
{
	GameObject::AI();//测试用，MortalObject不用于实例化对象，则此行可去除
	setHealth(getHealth() + getHealthReg());
	checkHealth();
	if (isDestroyed && ObjectType==TEST_MOR_OBJ)
	{
		onDestroy();
	}
}

bool  MortalObject::checkHealth()
{
	if (health > max_health)
	{
		health = max_health;
		return false;
	}
	if (health <= 0 && !isDestroyed)
	{
		setHealthReg(0);
		isDestroyed = true;
		((HelloWorld*)(CCDirector::sharedDirector()->getRunningScene()))->unregGameObject(this);
		return true;
	}
	return false;
}

int MortalObject::onHurt(int change,int angle)
{
	setHealth(getHealth() + change);
	CCFiniteTimeAction* action;
	action = CCSequence::create(CCFadeTo::create(0.5, 120),
		CCFadeTo::create(0.25, 255),
		NULL);
	runAction(action);
	return 1;
}


long long MortalObject::setHealth(long long _health)
{
	health = _health;
	//checkHealth();
	return health;
}
long long MortalObject::getHealth()
{
	return health;
}
long long MortalObject::setMaxHealth(long long _max_health)
{
	return (max_health = _max_health);
}
long long MortalObject::getMaxHealth()
{
	return max_health;
}
int MortalObject::setHealthReg(int reg)
{
	healthReg = reg;
	return healthReg;
}
int MortalObject::getHealthReg()
{
	return healthReg;
}



MortalObject* MortalObject::create()
{
	MortalObject *pObject = new MortalObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}
MortalObject* MortalObject::create(const char* image)
{
	MortalObject *pObject = new MortalObject();
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}
