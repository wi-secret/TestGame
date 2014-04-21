#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;

class WelcomeScene : public CCScene
{
public:
	
	CCPoint origin;
	CCSize visibleSize;

	virtual bool init();

	static cocos2d::CCScene* scene();

	static WelcomeScene* create();

	CCActionInterval* action_p1;
	CCActionInterval* action_p2;
	CCActionInterval* action_p3;
	CCActionInterval* action_p4;
	CCActionInterval* action;

	CCParticleSystemQuad* particle_1;
	CCParticleSystemQuad* particle_2;

	CCSprite* project_logo;

	void cbShowParticle();

	void cbShowParticle_2();

	void cbHideParticle();

	void playAnimation();

	virtual void update(float dt);
};