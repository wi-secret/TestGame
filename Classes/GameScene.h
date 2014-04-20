#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\GameObject.h"
#include "proj.win32\MortalObject.h"
#include "proj.win32\MovableObject.h"
#include "proj.win32\BasicObject.h"
#include "proj.win32\SelfObject.h"
#include "proj.win32\ShieldedObject.h"
#include "proj.win32\Effect.h"
#include "proj.win32\e_speedlimit.h"
#include "proj.win32\e_sethealth.h"
#include "proj.win32\e_speedup.h"
#include "proj.win32\MyKeyboardControl.h"
#include "proj.win32\GameController.h"
#include <list>
#include <cmath>

class HelloWorld : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

private:

	SelfObject* selfUnit;
	BasicObject* enemy;
	cocos2d::CCSprite* s_background;
	ShieldedObject* shield_unit;
	ShieldedObject* shield_unit2;
	std::list<GameObject*> GameObjects;

	void redraw();
	static void bgRotateL(void*,void*);
	static void bgRotateR(void*,void*);
	static void bgGoUp(void*,void*);
	static void bgGoLeft(void*,void*);
	static void bgGoRight(void*,void*);
	static void bgGoDown(void*,void*);

	virtual void update(float dt);

	static bool isnullptr(const GameObject*);
public:
	void regGameObject(GameObject* pObject);
	void unregGameObject(GameObject* pObject);
};

#endif // __GMAE_SCENE_H__
