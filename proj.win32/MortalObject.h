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