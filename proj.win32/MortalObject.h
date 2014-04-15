#pragma once
#include "MovableObject.h"

#define STATIC_UT -1
#define DEFAULT_UT 0
#define HEAVY_UT 1
#define LIGHT_UT 2

class MortalObject :public MovableObject{
public:
	long long max_health;
	long long health;


	static MortalObject* create();
	static MortalObject* create(const char* image);


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