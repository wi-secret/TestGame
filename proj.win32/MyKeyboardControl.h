#pragma once
#include <map>
#include <list>
#include <vector>
#include <Windows.h>
#include <functional>

using namespace std;

typedef function<bool(void*)> onKeyDown;

//可靠的具有初始化的功能
typedef struct __cbHoldKeyFunc{
	onKeyDown onStart;
	onKeyDown onHold;
	onKeyDown onRelease;
	struct {
		void* start;
		void* hold;
		void* release;
	} userdata;

	__cbHoldKeyFunc() {
		onStart=[](void*){return true; };
		onHold=[](void*){return true; };
		onRelease=[](void*){return true; };
		userdata.start=nullptr;
		userdata.hold=nullptr;
		userdata.release=nullptr;
	}
} cbHoldKeyFunc;

typedef struct __cbCombinKeyFunc{
	map<unsigned char,__cbCombinKeyFunc*> nextKey;
	onKeyDown onHold;
	void* userdata;

	__cbCombinKeyFunc() {
		nextKey.clear();
		userdata=nullptr;
		onHold=[](void*){return true; };
	}
} cbCombinKeyFunc;


//所有pop函数均不支持在checkKeyState阶段调用

#define KEY_IDLE 0
#define KEY_DOWN 1
#define KEY_HOLD 2
#define KEY_RELEASE 3

#define ADD_COMBIN_VIA 0
#define ADD_COMBIN_CLEAR 1
class MyKeyboardControl
{
public:

	~MyKeyboardControl();
	bool isActive();
	void pushKeyCallback(int keyValue,onKeyDown inFunc,void *userdata);
	void popKeyCallback(int keyValue);
	void clearKeyMap();
	static MyKeyboardControl* getInstance();
	//结束程序时需调用release()
	static void release();
	//按键监视主函数
	void checkKeyState();

	//需要修改设定的时候setActivation(false),可以使得状态监测函数失效.
	void setActivation(bool);

	//初始化按键状态,设置完按键，开始监视按键的时候调用.
	void resetState();

	//现阶段长按和组合键只支持单个函数

	//长按的set,get,clear函数
	void pushHoldKeyCallback(int keyValue,onKeyDown onStart,void* startUserdata,onKeyDown onHold,void* holdUserdata,onKeyDown onRelease,void *releaseUserdata);
	void popHoldKeyCallback(int keyValue);
	void clearHoldKeyMap();

	//组合键的set,get,clear函数
	void pushCombinKeyCallback(vector<unsigned char>& keyOrder,onKeyDown cbFunc,void *userdata);
	//链式添加
	MyKeyboardControl* addCombinKey(unsigned char key,onKeyDown cbFunc,void* userdata);
	//添加起始
	MyKeyboardControl* addCombinKey();
	//添加中途操作
	MyKeyboardControl* addCombinKey(unsigned char key,int operate_code=ADD_COMBIN_VIA);
	void clearCombinKeyMap();
	
	//清除所有按键设定
	void clearAllKeyMap();
private:
	MyKeyboardControl();
	static MyKeyboardControl* theInstance;
	bool m_bisActive;
	map<unsigned char, list < pair <onKeyDown , void*> >*> keyMap;
	map<unsigned char, cbHoldKeyFunc> holdKeyMap;
	map<unsigned char,cbCombinKeyFunc*> combinKeyMap;
	
	void deleteCombinKeyMap(cbCombinKeyFunc*);
	void checkCombinKey();
	void checkHoldKey();

	//resetState()的辅助函数
	void resetCombinKeyState(cbCombinKeyFunc*);

	//辅助链式添加的变量
	cbCombinKeyFunc *crntAddPos;
	//辅助判定组合键按键顺序变量
	vector<cbCombinKeyFunc*> keyOrders;
	vector<unsigned char> vkeyOrders;

	map<unsigned char,int> keysStat;
};