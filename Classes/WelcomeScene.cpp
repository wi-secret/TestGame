#pragma once
#include "WelcomeScene.h"
#include "GameScene.h"

bool WelcomeScene::init()
{
	if (!CCScene::init())
	{
		return false;
	}
	particle_1 = NULL;
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	action_p1 = CCDelayTime::create(0.5f);
	action_p2 = CCFadeIn::create(1.0f);
	action_p3 = CCDelayTime::create(3.5f);
	action_p4 = CCFadeOut::create(0.8f);

	playAnimation();
	scheduleUpdate();
	return true;
}

void WelcomeScene::playAnimation()
{
	project_logo = CCSprite::create("¼«¶«project.png");
	project_logo->setAnchorPoint(ccp(0.5, 0.5));
	project_logo->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(project_logo, 1);
	project_logo->setOpacity(0);
	action = CCSequence::create(action_p1,
		action_p2,
		CCCallFunc::create(this, callfunc_selector(WelcomeScene::cbShowParticle_2)),
		action_p3,action_p4,NULL);
	action->retain();
	project_logo->runAction(action);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("24. Mission accomplishment¡¡(LEVEL CLEAR).mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("24. Mission accomplishment¡¡(LEVEL CLEAR).mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.2f);

}

void WelcomeScene::update(float dt)
{
	if (action->isDone())
	{
		unscheduleUpdate();
		action->release();
		CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
	}
	
}

WelcomeScene* WelcomeScene::create()
{
	WelcomeScene *pObject = new WelcomeScene();
	if (pObject && pObject->init())
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return NULL;
}
void WelcomeScene::cbShowParticle()
{
	particle_1 = CCParticleGalaxy::create();
	particle_2 = CCParticleGalaxy::create();
	particle_1->setTotalParticles(200);
	particle_1->setLife(2.0f);
	particle_1->setStartSize(40);
	particle_1->setEndSize(25);
	particle_1->setSpeed(-25);
	particle_1->setSpeedVar(4);
	particle_1->setTangentialAccel(-80);
	particle_1->setGravity(ccp(0.0f, 0.0f));
	particle_1->setEmissionRate(200.0f);
	particle_1->setPosition(ccp(origin.x + visibleSize.width / 2 - project_logo->getContentSize().width / 2 - 100,
		origin.y + visibleSize.height / 2));
	particle_1->runAction(CCSequence::create(CCMoveBy::create(1.0,ccp(200 + project_logo->getContentSize().width, 0)),
		CCCallFunc::create(this, callfunc_selector(WelcomeScene::cbHideParticle)),
		NULL));

	particle_2->setTotalParticles(200);
	particle_2->setLife(2.0f);
	particle_2->setStartSize(40);
	particle_2->setEndSize(25);
	particle_2->setSpeed(25);
	particle_2->setSpeedVar(4);
	particle_2->setGravity(ccp(0.0f, 0.0f));
	particle_2->setEmissionRate(200.0f);
	particle_2->setPosition(ccp(origin.x + visibleSize.width / 2 + project_logo->getContentSize().width / 2 + 100,
		origin.y + visibleSize.height / 2));
	particle_2->runAction(CCSequence::create(CCMoveBy::create(1.0, ccp(-200 - project_logo->getContentSize().width, 0)),
		CCCallFunc::create(this, callfunc_selector(WelcomeScene::cbHideParticle)),
		NULL));
	addChild(particle_1);
	addChild(particle_2);
}

void WelcomeScene::cbShowParticle_2()
{
	particle_1 = CCParticleFire::createWithTotalParticles(550);
	particle_2 = CCParticleFire::createWithTotalParticles(550);

	particle_1->setAngleVar(120);
	particle_1->setSpeed(45);
	particle_1->setSpeedVar(10);
	particle_1->setLife(2.0f);
	particle_1->setEmissionRate(550.0 / 2.0);
	particle_1->setPosition(ccp(origin.x + visibleSize.width / 2 - project_logo->getContentSize().width / 2 - 100,
		origin.y + visibleSize.height / 2));
	particle_1->runAction(CCSequence::create(CCMoveBy::create(0.8, ccp(project_logo->getContentSize().width, 0)),
		CCDelayTime::create(0.8f),
		CCCallFunc::create(this, callfunc_selector(WelcomeScene::cbHideParticle)),
		NULL));

	particle_2->setAngleVar(120);
	particle_2->setSpeed(45);
	particle_2->setSpeedVar(10);
	particle_2->setLife(2.0f);
	particle_2->setEmissionRate(550.0 / 2.0);
	particle_2->setPosition(ccp(origin.x + visibleSize.width / 2 + project_logo->getContentSize().width / 2 + 100,
		origin.y + visibleSize.height / 2));
	particle_2->runAction(CCSequence::create(CCMoveBy::create(0.8, ccp(-project_logo->getContentSize().width, 0)),
		CCDelayTime::create(0.8f),
		CCCallFunc::create(this, callfunc_selector(WelcomeScene::cbHideParticle)),
		NULL));
	addChild(particle_1);
	addChild(particle_2);

}

void WelcomeScene::cbHideParticle()
{
	particle_1->setPosition(ccp(-100, -100));
	particle_2->setPosition(ccp(-100, -100));
}