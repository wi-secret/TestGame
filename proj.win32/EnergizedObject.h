#pragma once
#include "GameObject.h"

typedef void(*cbPowerSup)(int power, void *userdata);

class EnergizedObject :virtual public GameObject
{
public:
	typedef struct {
		cbPowerSup onPower;
		float priority;//优先权
		int order;//顺次
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

	virtual void AI();//需重载，作用：调用分配能量的函数，以及进行电池充能

	float MaxEnergyStock;
	float EnergyStock;
	float EnergyReg;
	int EnergyAllocate();

	~EnergizedObject();
protected:
	virtual int onDestroy();

	EnergizedObject();
};