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

	static ShieldedObject* create();
	static ShieldedObject* create(const char* image);

	static std::map<int, cocos2d::CCAnimation *>* shieldAnimations;

	int playShieldAnimation(int angle, int damage);//根据角度和伤害改变动画尚未实现

	long long SetMaxShield(long long maxshield);
	long long GetMaxShield();

	int GetShieldReg();
	int SetShieldReg(int shieldreg);

	long long SetShield(long long shield);
	long long GetShield();
	
	virtual void AI();

	virtual int onHurt(int damage,int angle);

	bool checkShield();

	~ShieldedObject();

protected:

	ShieldedObject();

	static std::map<int, cocos2d::CCAnimation *>* getShieldAnimations();

	virtual int onDestroy();//由AI调用，将自己释放
};

