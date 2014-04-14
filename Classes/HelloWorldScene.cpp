#include "HelloWorldScene.h"

//for debug use include
#include <Windows.h>

USING_NS_CC;

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	/////////////////////////////
	/* This function needs rewrite by a specific controller*/
	MyGameController* controller=GameController::getInstance();
	controller->setToDefault();
	controller->changeSceneTo(MYGC_SCENE_GAME);
	controller->linkLogicKey(MYGC_GAME_LK_MU,bgGoUp,this);
	controller->linkLogicKey(MYGC_GAME_LK_MD,bgGoDown,this);
	controller->linkLogicKey(MYGC_GAME_LK_ML,bgGoLeft,this);
	controller->linkLogicKey(MYGC_GAME_LK_MR,bgGoRight,this);
	controller->linkLogicKey(MYGC_GAME_LK_RR,bgRotateR,this);
	controller->linkLogicKey(MYGC_GAME_LK_RL,bgRotateL,this);
	controller->changeSceneTo(MYGC_SCENE_GAME);
	/////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    s_background = CCSprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	s_background->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    // add the sprite as a child to this layer
    this->addChild(s_background, 0);


	selfUnit = SelfObject::create("self.png");

	CCPoint bgSize = s_background->getContentSize();
	selfUnit->setPosition(ccp(bgSize.x / 2, bgSize.y / 2));
	selfUnit->addEffect(new e_speedlimit(0));
	s_background->addChild(selfUnit);
	regGameObject(selfUnit);

	enemy = MortalObject::create("enemy.png", 20, HEAVY_UT);
	enemy->setPosition(ccp(20, 30));
	enemy->addEffect(new e_sethealth(0, -5));
	regGameObject(enemy);
	s_background->addChild(enemy);
	

	scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt) {
	for(list<GameObject*>::iterator i=GameObjects.begin();i!=GameObjects.end();i++) {
		(*i)->AI();
	}
	redraw();
}

void HelloWorld::redraw() {
	CCSize bgSize = s_background->getContentSize();
	s_background->setAnchorPoint(ccp(selfUnit->getPositionX() / bgSize.width, selfUnit->getPositionY() / bgSize.height));
	s_background->setRotation(-selfUnit->getTrueRotation());
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


void HelloWorld::bgRotateL(void* buttonInfo,void* in) {
	HelloWorld *theInst = (HelloWorld*)in;
	theInst->selfUnit->setTrueRotation(theInst->selfUnit->getTrueRotation() - 1);
}

void HelloWorld::bgRotateR(void* buttonInfo,void* in) {
	HelloWorld *theInst = (HelloWorld*)in;
	theInst->selfUnit->setTrueRotation(theInst->selfUnit->getTrueRotation()+1);
}

void HelloWorld::bgGoUp(void* buttonInfo,void* in) {
	HelloWorld *theInst = (HelloWorld*)in;
	theInst->selfUnit->addEffect(new e_speedup(9,E_SPDUP_DIRECT_UP));
}

void HelloWorld::bgGoDown(void* buttonInfo,void* in) {
	HelloWorld *theInst = (HelloWorld*)in;
	theInst->selfUnit->addEffect(new e_speedup(9,E_SPDUP_DIRECT_DOWN));
}

void HelloWorld::bgGoLeft(void* buttonInfo,void* in) {
	HelloWorld *theInst = (HelloWorld*)in;
	theInst->selfUnit->addEffect(new e_speedup(9,E_SPDUP_DIRECT_LEFT));
}

void HelloWorld::bgGoRight(void* buttonInfo,void* in) {
	HelloWorld *theInst = (HelloWorld*)in;
	theInst->selfUnit->addEffect(new e_speedup(9,E_SPDUP_DIRECT_RIGHT));
}

void HelloWorld::regGameObject(GameObject* pObject) {
	GameObjects.push_back(pObject);
}
