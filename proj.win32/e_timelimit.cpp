#pragma once
#include "e_timelimit.h"
#include "GameScene.h"
e_timelimit::e_timelimit(int _priority,unsigned int _frameLimit) :Effect(_priority)
{
	frameLimit = _frameLimit;
	frameCount = 0;
}

e_timelimit::~e_timelimit()
{
	if (backupCopy != NULL){
		((e_timelimit*)backupCopy)->backupCopy = NULL;
		delete (e_timelimit*)backupCopy;
		backupCopy = NULL;
	}
}

void e_timelimit::onStart()
{
	if (frameLimit == 0)
	{
		dynamic_cast<HelloWorld*>(CCDirector::sharedDirector()->getRunningScene())->unregGameObject(GetParent());
		GetParent()->getParent()->removeChild(GetParent(), false);
	}
}

void e_timelimit::execute()
{
	frameCount++;
	backup();
	if (frameCount >= frameLimit)
	{
		dynamic_cast<HelloWorld*>(CCDirector::sharedDirector()->getRunningScene())->unregGameObject(GetParent());
		//GetParent()->retain();//仅在debug时谨慎使用
		GetParent()->isDestroyed = true;
	}
}

bool e_timelimit::isEnd()
{
	if (frameCount >= frameLimit)
	{
		return true;
	}
	else
	{
		return false;
	}
}