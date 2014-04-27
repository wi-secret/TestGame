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


//����pop��������֧����checkKeyState�׶ε���

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

	//�ֽ׶γ�������ϼ�ֻ֧�ֵ�������
	void pushHoldKeyCallback(int keyValue,onKeyDown onStart,onKeyDown onHold,onKeyDown onRelease,void *userdata);
	void popHoldKeyCallback(int keyValue);
	void clearHoldKeyMap();

	void pushCombinKeyCallback(vector<unsigned char>& keyOrder,onKeyDown cbFunc,void *userdata);
	//��ʽ���
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
	//������ʽ��ӵı���
	cbCombinKeyFunc *crntAddPos;
	//�����ж���ϼ�����˳�����
	vector<cbCombinKeyFunc*> keyOrders;
	vector<unsigned char> vkeyOrders;
};