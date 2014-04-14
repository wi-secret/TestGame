#pragma once
#include <map>
#include <list>
#include <Windows.h>
using namespace std;

typedef bool(*onKeyDown)(void*);
class MyKeyboardControl
{
public:

	~MyKeyboardControl();
	bool isActive();
	void pushKeyCallback(int keyValue,onKeyDown inFunc,void *userdata);
	void popKeyCallback(int keyValue);
	void clearKeyMap();
	static MyKeyboardControl* getInstance();
	static void release();
	void checkKeyState();
	void setActivation(bool);
private:
	MyKeyboardControl();
	static MyKeyboardControl* theInstance;
	bool m_bisActive;
	map<char, list < pair <onKeyDown , void*> >*> keyMap;
};

