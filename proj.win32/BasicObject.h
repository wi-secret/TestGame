#pragma once
#include "MortalObject.h"
#include "ShieldedObject.h"
class BasicObject : public MortalObject, public MovableObject,public ShieldedObject
{
public:
	int unit_type;//������ʵ�������ࡣǣ�浽������Ϸ���ݣ�Ӱ��AI()ִ�з�ʽ
	int level;//�ȼ������ھ����������ޣ��Լ��ٶ�����(δʵ��)

	static BasicObject* create(const char* image, int level);
	static BasicObject* create(const char* image, int level, int unit_type);

	virtual void AI();

	virtual int onHurt(int damage,int angle);//��e_health::execute()���ã����Ŷ�����

	~BasicObject();

protected:
	BasicObject();

	virtual int onDestroy();//��AI���ã����𲥷Ŷ����������Լ��ͷ�
};