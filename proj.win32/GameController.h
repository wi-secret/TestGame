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
/*
*可以允许通过平台层级的controler获取/设置指定逻辑按键的物理按键信息
*物理按键信息应该通过一个智能指针类进行封装，并通过这个类型与一个统一的接口进行set/get
*将GameController基类写作关于这个智能指针类的模板，各平台分别派生自不同的特化模板
*游戏中设置按键的场景单独设置为一类，作为具体游戏controller的友元类/友元函数，通过上述的set/get函数进行按键设置
*/

typedef void (*onButtonDown)(void* buttonInfo,void* userdata);
using namespace std;
template <typename keyInfo> class GameController;
class Win32KeyboardController;
class MyGameController;
class Win32KeyControlInfo;


template <typename keyInfo>
class GameController
{
public:
	
	//添加或修改对应逻辑按键的响应函数
	virtual void linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata);
	//保存按键设置（即实际按键到逻辑按键的映射）
	virtual bool saveConfig()=0;
	//载入按键设定
	virtual bool loadConfig()=0;
	//将按键设定设置为默认
	void setToDefault();
	//修复按键设定，即将没有实际按键相对的逻辑按键设为默认按键
	void repairKeyConfig();
	//更改按键场景,支持热切换
	void changeSceneTo(int sceneCode);
	//获取指定按键的物理按键信息，如果对应逻辑按键不存在返回false
	virtual bool getLogicKeyInfo(int logickey,keyInfo& info)=0;
	//设定指定按键的物理按键，如果没有被设定过，就添加新的logickey信息
	virtual void setLogicKeyInfo(int logickey,keyInfo& hard)=0;
	//设置是否处于活动状态，无法在活动状态更改当前场景按键
	void enable(bool);
	bool getActivation();

protected:
	virtual void linkHardwareKey() = 0;
	virtual void dislinkHardwareKey() = 0;
	void release();
	map<int, map <int, pair<onButtonDown, void*> > > cbFuncMaps;
	map<int, map<int,keyInfo>> defaultKeyMap;
	int crntScene;
private:
	GameController();
	bool activation;
	friend Win32KeyboardController;
};

class Win32KeyboardController : public GameController<Win32KeyControlInfo> {
public:
	//重新实现这两个函数
	virtual bool saveConfig();
	virtual bool loadConfig();

	virtual bool getLogicKeyInfo(int logicKey,Win32KeyControlInfo& info);
	virtual void setLogicKeyInfo(int logicKey,Win32KeyControlInfo& info);
protected:
	virtual void linkHardwareKey();
	virtual void dislinkHardwareKey();

	const char* configFileStorage;

private:
	
	//[场景][逻辑按键]->物理按键
	map<int, map<int, int> > hardwareKeyMaps;

	//[场景][逻辑按键]->{first:物理按键,second:状态}
	map < int, map<int, pair<int, int>>> holdHardwareKeyMap;

	//[场景][逻辑按键]->物理按键顺序
	map < int, map<int,vector<unsigned char>>> combinHardwareKeyMap;
};

class MyGameController : public Win32KeyboardController {
public:
	//获得单例对象指针
	static MyGameController* getInstance();
	MyGameController();
	~MyGameController();
private:
	static MyGameController* theInstance;
};

#define WIN32KEYTYPE_NULL 0
#define WIN32KEYTYPE_HOLD 1
#define WIN32KEYTYPE_COMBIN 2
#define WIN32KEYTYPE_KEY 3

#define WIN32HOLDTYPE_START 1
#define WIN32HOLDTYPE_HOLD 2
#define WIN32HOLDTYPE_RELEASE 3

class Win32KeyControlInfo{
public:
	Win32KeyControlInfo();
	~Win32KeyControlInfo();
	bool getAsCombinKey(vector<unsigned char> *keyOrders);
	void setCombinKey(vector<unsigned char>& keyOrders);
	//链式添加
	Win32KeyControlInfo* setCombinKey(int vkey=-1);

	bool getAsHoldKey(int *vkey, int *keyType);
	void setAsHoldKey(int vkey,int keyType);

	bool getAsKey(int *vkey);
	void setAsKey(int vkey);

	//返回值为结果的长度，不计'\0'
	//TODO:实现这两个函数
	int getAsString(char *res=NULL);
	int getAsString(string& res);

	int getType();

	void release();
private:
	typedef struct {
		int vkey;
		int keyState;
	} holdKeyInfo;

	int keytype;
	void *data;
};
