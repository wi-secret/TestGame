#pragma once
#include "GameObject.h"


class MovableObject :  public GameObject {
public:
	CCPoint speed;
	double palstance;//½ÇËÙ¶È

	static MovableObject* create();
	static MovableObject* create(const char* image);

	void setTrueRotation(float nRotation);
	float getTrueRotation();//½Ç¶È

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
