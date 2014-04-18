#pragma once
#include "GameObject.h"

#define DEFAULT_SHT 0
#define WHOLE_SHIELD -1

class ShieldedObject :virtual public GameObject
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

	long long setMaxShield(long long maxshield);
	long long getMaxShield();

	int getShieldReg();
	int setShieldReg(int shieldreg);

	long long setShield(long long shield);
	long long getShield();
	
	virtual void AI();

	virtual int onHurt(int damage,int angle);

	bool checkShield();

	~ShieldedObject();

	CCSprite* node;

protected:

	ShieldedObject();

	static std::map<int, cocos2d::CCAnimation *>* getShieldAnimations();

	virtual int onDestroy();//由AI调用，将自己释放
};

