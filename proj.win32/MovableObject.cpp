#include "MovableObject.h"

MovableObject* MovableObject::create() {
	MovableObject *pObject = new MovableObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

MovableObject* MovableObject::create(const char* pszFileName) {
	MovableObject *pObject = new MovableObject();
	if (pObject && pObject->initWithFile(pszFileName))
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

void MovableObject::AI() {
	GameObject::AI();
	setPosition(ccp(getPositionX() + speed.x, getPositionY() + speed.y));
	setTrueRotation(getTrueRotation() + palstance);
}

void MovableObject::setTrueRotation(float nRotation) {
	setRotation(getRotation() + nRotation - rotation);
	rotation = nRotation;
}

float MovableObject::getTrueRotation() {
	return rotation;
}

int MovableObject::onDestroy() {
	return 0;
}

MovableObject::MovableObject() {
	rotation = 0;
	palstance = 0;
	speed = ccp(0, 0);
}

void MovableObject::addLocalSpeed(CCPoint& addSpeed) {
	double Cos = cos(-rotation*M_PI / 180);
	double Sin = sin(-rotation*M_PI / 180);
	speed.x += addSpeed.x*Cos - addSpeed.y*Sin;
	speed.y += addSpeed.x*Sin + addSpeed.y*Cos;
}

void MovableObject::setLocalSpeedX(double newSpeedX) {
	static double Cos_old;
	static double Sin_old;
	double Cos = cos(rotation*M_PI / 180);
	double Sin = sin(rotation*M_PI / 180);
	double localSpeedY = 0;//
	localSpeedY = speed.x*Sin_old + speed.y*Cos_old;
	Cos_old = Cos;
	Sin_old = Sin;
	speed.x = localSpeedY*Sin + newSpeedX*Cos;
	speed.y = localSpeedY*Cos - newSpeedX*Sin;
}

void MovableObject::setLocalSpeedY(double newSpeedY) {
	static double Cos_old;
	static double Sin_old;
	double Cos = cos(rotation*M_PI / 180);
	double Sin = sin(rotation*M_PI / 180);
	double localSpeedX = 0;// speed.x*Cos - speed.y*Sin;
	localSpeedX = speed.x*Cos_old - speed.y*Sin_old;
	Cos_old = Cos;
	Sin_old = Sin;
	speed.x = Cos*localSpeedX + newSpeedY*Sin;
	speed.y = -Sin*localSpeedX + newSpeedY*Cos;
}

MovableObject::~MovableObject(){
}

void MovableObject::setLocalSpeed(CCPoint& newSpeed) {
	double Cos = cos(-rotation*M_PI / 180);
	double Sin = sin(-rotation*M_PI / 180);
	speed.x = newSpeed.x*Cos - newSpeed.y*Sin;
	speed.y = newSpeed.x*Sin + newSpeed.y*Cos;
}

double MovableObject::getLocalSpeedX() {
	return speed.x*cos(-rotation*M_PI / 180) + speed.y*sin((-rotation*M_PI / 180));
}

double MovableObject::getLocalSpeedY() {
	return -speed.x*sin(-rotation*M_PI / 180) + speed.y*cos(-rotation*M_PI / 180);
}
