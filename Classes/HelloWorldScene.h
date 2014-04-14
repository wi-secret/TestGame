#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "proj.win32\GameObject.h"
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
	MortalObject* enemy;
	cocos2d::CCSprite *s_background;

	std::list<GameObject*> GameObjects;

	void redraw();
	static void bgRotateL(void*,void*);
	static void bgRotateR(void*,void*);
	static void bgGoUp(void*,void*);
	static void bgGoLeft(void*,void*);
	static void bgGoRight(void*,void*);
	static void bgGoDown(void*,void*);

	void regGameObject(GameObject* pObject);
	virtual void update(float dt);
};

#endif // __HELLOWORLD_SCENE_H__
