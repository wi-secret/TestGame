#include "SelfObject.h"

SelfObject* SelfObject::create() {
	SelfObject *pObject = new SelfObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

SelfObject* SelfObject::create(const char* pszFileName) {
	SelfObject *pObject = new SelfObject();
	if (pObject && pObject->initWithFile(pszFileName))
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

