#pragma once
#include "MortalObject.h"
#include "HelloWorldScene.h"

MortalObject::MortalObject()
{
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
	this->setVisible(false);
	//((HelloWorld*)(this->getParent()->getParent()))->unregGameObject(this);
	((HelloWorld*)(CCDirector::sharedDirector()->getRunningScene()))->unregGameObject(this);
	this->getParent()->removeChild(this, true);
}

int MortalObject::onDestroy()
{
	
	static CCBlink* action;
	if (isDestroyed){
		if (action->isDone())
		{
			action->autorelease();
			cbDestroy();
		}
	}
	if (isDestroyed)
		return 0;
	action = new CCBlink;
	action->initWithDuration(1, 3);
	runAction(action);
	isDestroyed = true;
	return 1;
}

void MortalObject::AI()
{
	GameObject::AI();
	setHealth(getHealth() + getHealthReg());
	checkHealth();
}

bool  MortalObject::checkHealth()
{
	if (health > max_health)
	{
		health = max_health;
		return false;
	}
	if (health <= 0)
	{
		setHealthReg(0);
		onDestroy();
	}
	return true;
}

int MortalObject::onHurt(int change,int angle)
{
	health += change;
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
