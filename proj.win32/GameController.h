#pragma once
#include <map>
#include <vector>
#include "tinyxml\tinyxml.h"
#include "MyKeyboardControl.h"


#define MYGC_SCENE_MENU 0
#define MYGC_SCENE_GAME 1

#define MYGC_GAME_LK_ML 0
#define MYGC_GAME_LK_MR 1
#define MYGC_GAME_LK_MU 2
#define MYGC_GAME_LK_MD 3
#define MYGC_GAME_LK_RL 4
#define MYGC_GAME_LK_RR 5


typedef void (*onButtonDown)(void* buttonInfo,void* userdata);
using namespace std;
class GameController;
class Win32KeyboardController;
class MyGameController;

class GameController
{
public:
	//获得单例对象指针
	static MyGameController* getInstance();
	
	//添加或修改对应逻辑按键的响应函数
	virtual void linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata);
	//保存按键设置（即实际按键到逻辑按键的映射）
	virtual bool saveConfig()=0;
	//载入按键设定
	virtual bool loadConfig()=0;
	//将按键设定设置为默认
	virtual void setToDefault() = 0;
	//修复按键设定，即将没有实际按键相对的逻辑按键设为默认按键
	virtual void repairKeyConfig() = 0;
	//更改按键场景
	void changeSceneTo(int sceneCode);

protected:
	virtual void linkHardwareKey() = 0;
	virtual void dislinkHardwareKey() = 0;
	void release();
	map<int, map <int, pair<onButtonDown, void*> > > cbFuncMaps;
	static MyGameController* theInstance;
	int crntScene;
private:
	GameController();
	friend Win32KeyboardController;
};

class Win32KeyboardController : public GameController {
public:
	virtual bool saveConfig();
	virtual bool loadConfig();
	virtual void setToDefault();
	virtual void repairKeyConfig();
	//获得对应逻辑按键的虚拟键码值
	int getvKey(int scene,int vlogic);
protected:
	virtual void linkHardwareKey();
	virtual void dislinkHardwareKey();

	static bool cbGeneralOnKeyDown(void* vLogicKey);

	map<int, map<int, int> > hardwareKeyMaps;
	map<int, map<int, int> > defualtKeyMaps;
	const char* configFileStorage;
private:
	vector<int*> vlKey;
};

class MyGameController : public Win32KeyboardController {
public:
	MyGameController();
	~MyGameController();
};