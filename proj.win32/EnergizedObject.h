#pragma once
#include "GameObject.h"

typedef void(*cbPowerSup)(int power, void *userdata);

class EnergizedObject :virtual public GameObject
{
public:
	typedef struct {
		cbPowerSup onPower;
		float priority;//����Ȩ
		int order;//˳��
		float minEnergy;
		void *userdata;
	} powerUnit;

	std::list<std::list<powerUnit>> powerUnits;

	static EnergizedObject* create();

	float getMaxEnergyStock();
	float setMaxEnergyStock(float maxenergy);
	float getEnergyStock();
	float setEnergyStock(float energy);
	float getEnergyReg();
	float setEnergyReg(float energyReg);

	int checkEnergy();

	virtual void AI();//�����أ����ã����÷��������ĺ������Լ����е�س���

	float MaxEnergyStock;
	float EnergyStock;
	float EnergyReg;
	int EnergyAllocate();

	~EnergizedObject();
protected:
	virtual int onDestroy();

	EnergizedObject();
};