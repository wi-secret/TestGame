#include "GameObject.h"


GameObject::GameObject(void)
{
	maxTag=0;
}


GameObject::~GameObject(void)
{
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		delete (*i);
	}
}

int GameObject::addEffect(Effect* effect) {
	pEffectArray.push_back(effect);
	effect->tag=maxTag;
	effect->parent=this;
	(effect->*(effect->__onStart))();
	effect->backup();
	maxTag++;
	pEffectArray.sort(GameObject::effectPrioritySort);
	return maxTag-1;
}

Effect* GameObject::getEffect(int effectID) {
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if((*i)->tag==effectID) {
			return *i;
		}
	}
	return NULL;
}

bool GameObject::removeEffect(int effectID) {
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if((*i)->tag==effectID) {
			delete (*i);
			pEffectArray.erase(i);
			return true;
		}
	}
	return false;
}

void GameObject::foreachEffect(forEffect f,void* userdata) {
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if(!f(*i,(*i)->tag,userdata)) {
			return;
		}
	}
}

void GameObject::AI() {
	list<int> removeList;
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if(((*i)->*((*i)->__isEnd))()) {
			removeList.push_back((*i)->tag);
		} else {
			((*i)->*((*i)->__execute))();
		}
	}
	for(list<int>::iterator i=removeList.begin();i!=removeList.end();i++) {
		removeEffect(*i);
	}
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		(*i)->recovery();
	}
}

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
	setPosition(ccp(getPositionX()+speed.x,getPositionY()+speed.y));
	setTrueRotation(getTrueRotation()+palstance);
}

void MovableObject::setTrueRotation(float nRotation) {
	setRotation(getRotation()+nRotation-rotation);
	rotation=nRotation;
}

float MovableObject::getTrueRotation() {
	return rotation;
}

int MovableObject::onDestroy() {
	return 0;
}

MovableObject::MovableObject() {
	rotation=0;
	palstance=0;
	speed=ccp(0,0);
}

void MovableObject::addLocalSpeed(CCPoint& addSpeed) {
	double Cos=cos(-rotation*M_PI/180);
	double Sin=sin(-rotation*M_PI/180);
	speed.x+=addSpeed.x*Cos-addSpeed.y*Sin;
	speed.y+=addSpeed.x*Sin+addSpeed.y*Cos;
}

void MovableObject::setLocalSpeedX(double newSpeedX) {
	static double Cos_old;
	static double Sin_old;
	double Cos = cos(rotation*M_PI / 180);
	double Sin = sin(rotation*M_PI / 180);
	double localSpeedY = 0;//
	localSpeedY=speed.x*Sin_old + speed.y*Cos_old;
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
	return speed.x*cos(-rotation*M_PI/180)+speed.y*sin((-rotation*M_PI/180));
}

double MovableObject::getLocalSpeedY() {
	return -speed.x*sin(-rotation*M_PI/180)+speed.y*cos(-rotation*M_PI/180);
}

SelfObject* SelfObject::create() {
	SelfObject *pObject = new SelfObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

SelfObject* SelfObject::create(const char* pszFileName) {
	SelfObject *pObject = new SelfObject();
	if (pObject && pObject->initWithFile(pszFileName))
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}

bool GameObject::effectPrioritySort(Effect *x,Effect *y) {
		return x->priority>y->priority;
}
/*
* ÉúÃüÀà
* 
*/
MortalObject::MortalObject()
{
	max_health = 1;
	health = 1;
	isDestroyed = 0;
	level = 1;
	unit_type = DEFAULT_UT;
}

MortalObject::~MortalObject()
{
}

void MortalObject::cbDestroy()
{
	this->setVisible(false);
	//this->getParent()->removeChild(this, false);
}

int MortalObject::onDestroy()
{
	if (isDestroyed)
		return 0;
	CCFiniteTimeAction* action;
	action = CCSequence::create( CCBlink::create(3, 10),
		CCCallFunc::create(this,callfunc_selector(MortalObject::cbDestroy)),
		NULL);
	runAction(action);
	isDestroyed = 1;
	return 1;
}

void MortalObject::AI()
{
	if (unit_type == STATIC_UT)
		GameObject::AI();
	else MovableObject::AI();
	switch (unit_type)
	{
	case HEAVY_UT:
		//addEffect(new e_heavyU);
		break;
	case LIGHT_UT:
		//addEffect(new e_lightU);
		break;
	case DEFAULT_UT:
		break;
	default:
		break;
	}
	speed.x = 0.2;
	speed.y = 0.3;
	check();
}

bool  MortalObject::check()
{
	if (health > max_health)
	{
		health = max_health;
		return false;
	}
	if (health <= 0)
		onDestroy();
	return true;
}

int MortalObject::onHurt()
{
	
	CCFiniteTimeAction* action;
	action = CCSequence::create(CCFadeTo::create(0.25, 0),
		CCFadeTo::create(0.25, 255),
		NULL);
	runAction(action);
	return 1;
}

MortalObject* MortalObject::create()
{
	MortalObject *pObject = new MortalObject();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}
MortalObject* MortalObject::create(const char* image)
{
	MortalObject *pObject = new MortalObject();
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}
MortalObject* MortalObject::create(const char* image, int level)
{
	MortalObject *pObject = new MortalObject();
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		pObject->level = level;
		pObject->health = pObject->max_health = level*15;
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}
MortalObject* MortalObject::create(const char* image, int level, int unit_type)
{
	MortalObject *pObject = new MortalObject();
	if (pObject && pObject->initWithFile(image))
	{
		pObject->autorelease();
		pObject->level = level;
		pObject->unit_type = unit_type;
		switch (unit_type)
		{
		case HEAVY_UT:
			pObject->health = pObject->max_health = level * 25;
			break;
		case LIGHT_UT:
			pObject->health = pObject->max_health = level * 10;
			break;
		default:
			pObject->health = pObject->max_health = level * 15;
			break;
		}
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}