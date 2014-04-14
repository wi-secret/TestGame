#pragma once
#include "cocos2d.h"
#include "Effect.h"
#include <list>
#include <algorithm>

#define STATIC_UT -1
#define DEFAULT_UT 0
#define HEAVY_UT 1
#define LIGHT_UT 2

using namespace std;
using namespace cocos2d;
class Effect;

typedef bool (*forEffect)(Effect* effect,int effectID, void* userdata);

class GameObject :
	public CCSprite
{
public:
	//effect�����set,get�ͱ���
	//GameObject�����ͷ�effect
	int addEffect(Effect* effect);
	Effect* getEffect(int effectID);
	//�Ƴ�effect��ʱ�����ʹ�øú�����ֹ�ڴ�й¶
	bool removeEffect(int effectID);
	//f����false��ʱ���ж�ѭ��
	void foreachEffect(forEffect f,void* userdata);
	
	virtual void AI();
	
	~GameObject(void);
	
protected:
	virtual int onDestroy()=0;
	
	GameObject(void);
private:
	list<Effect*> pEffectArray;
	unsigned int maxTag;

	static bool effectPrioritySort(Effect*,Effect*);
};

class MovableObject : public GameObject {
public:
	CCPoint speed;
	double palstance;//���ٶ�
	
	static MovableObject* create();
	static MovableObject* create(const char* image);

	void setTrueRotation(float nRotation);
	float getTrueRotation();//�Ƕ�

	void setLocalSpeed(CCPoint& newSpeed);
	void addLocalSpeed(CCPoint& addSpeed);
	void setLocalSpeedX(double newSpeedX);
	void setLocalSpeedY(double newSpeedY);
	double getLocalSpeedX();
	double getLocalSpeedY();

	
	virtual void AI();

	~MovableObject();
protected:
	virtual int onDestroy();

	MovableObject();
private:
	float rotation;
};

class SelfObject: public MovableObject{
public:
	static SelfObject* create();
	static SelfObject* create(const char* image);
};

class MortalObject :public MovableObject{
public:
	long long max_health;
	long long health;
	int unit_type;//������ʵ�������ࡣǣ�浽������Ϸ���ݣ�Ӱ��AI()ִ�з�ʽ
	int level;//�ȼ������ھ����������ޣ��Լ��ٶ�����(δʵ��)

	static MortalObject* create();
	static MortalObject* create(const char* image);
	static MortalObject* create(const char* image, int level);
	static MortalObject* create(const char* image, int level, int unit_type);

	virtual void AI();

	bool check();

	virtual int onHurt();//��e_health::execute()���ã����Ŷ�����

	int isDestroyed;

	void cbDestroy();

	~MortalObject();
protected:

	virtual int onDestroy();//��AI���ã����𲥷Ŷ����������Լ��ͷ�

	MortalObject();

};