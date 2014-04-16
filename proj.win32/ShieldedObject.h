#pragma once
#include "GameObject.h"

#define DEFAULT_SHT 0
#define WHOLE_SHIELD -1
class ShieldedObject : public GameObject
{

public:

	long long MaxShield;
	long long Shield;
	int ShieldType;
	double ShieldEfficiency;
	int ShieldRegenration;

	static std::map<int, cocos2d::CCAnimation *>* shieldAnimations;

	int playShieldAnimation(int angle, int damage);

	long long SetMaxShield(long long maxshield);
	long long GetMaxShield();

	int GetShieldReg();
	int SetShieldReg(int shieldreg);

	long long SetShield(long long shield);
	long long GetShield();

	static std::map<int, cocos2d::CCAnimation *>* getShieldAnimations();

	~ShieldedObject();

protected:

	ShieldedObject();
	
	
};
std::map<int, cocos2d::CCAnimation *>* ShieldedObject::shieldAnimations;