#pragma once
#include "BasicObject.h"

BasicObject* BasicObject::create(const char* image, int level)
{
	BasicObject *pObject = new BasicObject();
	pObject->node = new CCSprite;
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		pObject->level = level;
		pObject->setHealth(pObject->max_health = level * 15);
		pObject->node->init();
		pObject->node->autorelease();
		pObject->addChild(pObject->node);
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

BasicObject* BasicObject::create(const char* image, int level, int unit_type)
{
	BasicObject *pObject = new BasicObject();
	pObject->node = new CCSprite;
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		pObject->level = level;
		pObject->unit_type = unit_type;
		switch (unit_type)
		{
		case HEAVY_UT:
			pObject->setHealth(pObject->max_health = level * 25);
			break;
		case LIGHT_UT:
			pObject->setHealth(pObject->max_health = level * 10);
			break;
		default:
			pObject->setHealth(pObject->max_health = level * 15);
			break;
		}
		pObject->node->init();
		pObject->node->autorelease();
		pObject->addChild(pObject->node);
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

BasicObject::BasicObject()
{
	level = 1;
	unit_type = DEFAULT_UT;
}

int BasicObject::onHurt(int damage,int angle)
{
	ShieldedObject::onHurt(damage, angle);
	return 1;
}

BasicObject::~BasicObject()
{

}

int BasicObject::onDestroy()
{
	MortalObject::onDestroy();
	return 1;
}

void BasicObject::AI()
{
	speed.x = 0.1;
	speed.y = 0.13;
	ShieldedObject::AI();
	MortalObject::AI();
}