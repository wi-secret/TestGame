#pragma once
#include "GameObject.h"

#define DEFAULT_SHT 0
#define WHOLE_SHIELD -1
class ShieldedObject : public GameObject
{

public:

	int MaxShield;
	int Shield;
	int ShieldType;
	double ShieldEfficiency;
	int ShieldRegenration;

	static std::map<int, cocos2d::CCAnimation *>* shieldAnimations;

	int playShieldAnimation(int angle, int damage);

	int SetMaxShield();
	int GetMaxShield();

	int GetShieldReg();
	int SetShieldReg();

	int SetShield();
	int GetShield();

	static std::map<int, cocos2d::CCAnimation *>* getShieldAnimations();

	~ShieldedObject();

protected:

	ShieldedObject();
	
	
};
std::map<int, cocos2d::CCAnimation *>* ShieldedObject::shieldAnimations;