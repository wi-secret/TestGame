#pragma once
#include <map>
#include <list>
#include <vector>
#include <Windows.h>
using namespace std;

typedef bool(*onKeyDown)(void*);
typedef struct {
	onKeyDown onStart;
	onKeyDown onHold;
	onKeyDown onRelease;
	void* userdata;
} cbHoldKeyFunc;

typedef struct __cbCombinKeyFunc{
	map<unsigned char,__cbCombinKeyFunc*> nextKey;
	onKeyDown onHold;
	void* userdata;
} cbCombinKeyFunc;


//所有pop函数均不支持在checkKeyState阶段调用

#define KEY_IDLE 0
#define KEY_DOWN 1
#define KEY_HOLD 2
#define KEY_RELEASE 3

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
	void checkKeyState();

	//需要修改设定的时候setActivation(false),可以使得状态监测函数失效.
	void setActivation(bool);

	//初始化按键状态,设置完按键，开始监视按键的时候调用.
	void resetState();

	//现阶段长按和组合键只支持单个函数
	void pushHoldKeyCallback(int keyValue,onKeyDown onStart,onKeyDown onHold,onKeyDown onRelease,void *userdata);
	void popHoldKeyCallback(int keyValue);
	void clearHoldKeyMap();

	void pushCombinKeyCallback(vector<unsigned char>& keyOrder,onKeyDown cbFunc,void *userdata);
	//链式添加
	MyKeyboardControl* addCombinKey(unsigned char key=0,onKeyDown cbFunc=NULL,void* userdata=NULL);
	static bool clear_combin_key_func(void*);
	void clearCombinKeyMap();

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