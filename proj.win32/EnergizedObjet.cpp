#pragma once
#include "EnergizedObject.h"

EnergizedObject::EnergizedObject()
{
	MaxEnergyStock = 1;
	EnergyStock = 1;
	EnergyReg = 0;
}

EnergizedObject::~EnergizedObject()
{

}

void EnergizedObject::AI()
{
	GameObject::AI();
	setEnergyStock(getEnergyStock() + getEnergyReg());
	checkEnergy();
	EnergyAllocate();
}

float EnergizedObject::getMaxEnergyStock()
{
	return MaxEnergyStock;
}

float EnergizedObject::getEnergyStock()
{
	return EnergyStock;
}

float EnergizedObject::getEnergyReg()
{
	return EnergyReg;
}

float EnergizedObject::setMaxEnergyStock(float maxenergy)
{
	MaxEnergyStock = maxenergy;
	return MaxEnergyStock;
}

float EnergizedObject::setEnergyReg(float energyReg)
{
	EnergyReg = energyReg;
	return EnergyReg;
}

float EnergizedObject::setEnergyStock(float energy)
{
	EnergyStock = energy;
	checkEnergy();
	return EnergyStock;
}

int EnergizedObject::onDestroy()
{

	return 1;
}

int EnergizedObject::checkEnergy()
{
	if (EnergyStock > MaxEnergyStock)
	{
		EnergyStock = MaxEnergyStock;
		return 1;
	}
	if (EnergyStock < 0)
	{
		EnergyStock = 0;
		return -1;
	}
	return 1;
}

int EnergizedObject::EnergyAllocate()
{
	return 1;
}

EnergizedObject* EnergizedObject::create()
{
	EnergizedObject *pObject = new EnergizedObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}