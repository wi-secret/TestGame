#include "MortalObject.h"


MortalObject::MortalObject()
{
	max_health = 1;
	health = 1;
	isDestroyed = 0;
}

MortalObject::~MortalObject()
{
}

void MortalObject::cbDestroy()
{
	this->setVisible(false);
	//this->getParent()->removeChild(this, false);
}

int MortalObject::onDestroy()
{
	if (isDestroyed)
		return 0;
	CCFiniteTimeAction* action;
	action = CCSequence::create(CCBlink::create(3, 10),
		CCCallFunc::create(this, callfunc_selector(MortalObject::cbDestroy)),
		NULL);
	runAction(action);
	isDestroyed = 1;
	return 1;
}

void MortalObject::AI()
{
	MovableObject::AI();
	
	check();
}

bool  MortalObject::check()
{
	if (health > max_health)
	{
		health = max_health;
		return false;
	}
	if (health <= 0)
		onDestroy();
	return true;
}

int MortalObject::onHurt()
{
	CCFiniteTimeAction* action;
	action = CCSequence::create(CCFadeTo::create(0.25, 0),
		CCFadeTo::create(0.25, 255),
		NULL);
	runAction(action);
	return 1;
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
