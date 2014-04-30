#pragma once
#include <map>
#include <list>
#include <vector>
#include <Windows.h>
#include <functional>

using namespace std;

typedef function<bool(void*)> onKeyDown;

//�ɿ��ľ��г�ʼ���Ĺ���
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


//����pop��������֧����checkKeyState�׶ε���

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
	//��������ʱ�����release()
	static void release();
	//��������������
	void checkKeyState();

	//��Ҫ�޸��趨��ʱ��setActivation(false),����ʹ��״̬��⺯��ʧЧ.
	void setActivation(bool);

	//��ʼ������״̬,�����갴������ʼ���Ӱ�����ʱ�����.
	void resetState();

	//�ֽ׶γ�������ϼ�ֻ֧�ֵ�������

	//������set,get,clear����
	void pushHoldKeyCallback(int keyValue,onKeyDown onStart,void* startUserdata,onKeyDown onHold,void* holdUserdata,onKeyDown onRelease,void *releaseUserdata);
	void popHoldKeyCallback(int keyValue);
	void clearHoldKeyMap();

	//��ϼ���set,get,clear����
	void pushCombinKeyCallback(vector<unsigned char>& keyOrder,onKeyDown cbFunc,void *userdata);
	//��ʽ���
	MyKeyboardControl* addCombinKey(unsigned char key,onKeyDown cbFunc,void* userdata);
	//�����ʼ
	MyKeyboardControl* addCombinKey();
	//�����;����
	MyKeyboardControl* addCombinKey(unsigned char key,int operate_code=ADD_COMBIN_VIA);
	void clearCombinKeyMap();
	
	//������а����趨
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

	//resetState()�ĸ�������
	void resetCombinKeyState(cbCombinKeyFunc*);

	//������ʽ��ӵı���
	cbCombinKeyFunc *crntAddPos;
	//�����ж���ϼ�����˳�����
	vector<cbCombinKeyFunc*> keyOrders;
	vector<unsigned char> vkeyOrders;

	map<unsigned char,int> keysStat;
};