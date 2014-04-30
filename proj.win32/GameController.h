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
*��������ͨ��ƽ̨�㼶��controler��ȡ/����ָ���߼���������������Ϣ
*��������ϢӦ��ͨ��һ������ָ������з�װ����ͨ�����������һ��ͳһ�Ľӿڽ���set/get
*��GameController����д�������������ָ�����ģ�壬��ƽ̨�ֱ������Բ�ͬ���ػ�ģ��
*��Ϸ�����ð����ĳ�����������Ϊһ�࣬��Ϊ������Ϸcontroller����Ԫ��/��Ԫ������ͨ��������set/get�������а�������
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
	
	//��ӻ��޸Ķ�Ӧ�߼���������Ӧ����
	virtual void linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata);
	//���水�����ã���ʵ�ʰ������߼�������ӳ�䣩
	virtual bool saveConfig()=0;
	//���밴���趨
	virtual bool loadConfig()=0;
	//�������趨����ΪĬ��
	void setToDefault();
	//�޸������趨������û��ʵ�ʰ�����Ե��߼�������ΪĬ�ϰ���
	void repairKeyConfig();
	//���İ�������,֧�����л�
	void changeSceneTo(int sceneCode);
	//��ȡָ����������������Ϣ�������Ӧ�߼����������ڷ���false
	virtual bool getLogicKeyInfo(int logickey,keyInfo& info)=0;
	//�趨ָ�������������������û�б��趨����������µ�logickey��Ϣ
	virtual void setLogicKeyInfo(int logickey,keyInfo& hard)=0;
	//�����Ƿ��ڻ״̬���޷��ڻ״̬���ĵ�ǰ��������
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
	//����ʵ������������
	virtual bool saveConfig();
	virtual bool loadConfig();

	virtual bool getLogicKeyInfo(int logicKey,Win32KeyControlInfo& info);
	virtual void setLogicKeyInfo(int logicKey,Win32KeyControlInfo& info);
protected:
	virtual void linkHardwareKey();
	virtual void dislinkHardwareKey();

	const char* configFileStorage;

private:
	
	//[����][�߼�����]->������
	map<int, map<int, int> > hardwareKeyMaps;

	//[����][�߼�����]->{first:������,second:״̬}
	map < int, map<int, pair<int, int>>> holdHardwareKeyMap;

	//[����][�߼�����]->������˳��
	map < int, map<int,vector<unsigned char>>> combinHardwareKeyMap;
};

class MyGameController : public Win32KeyboardController {
public:
	//��õ�������ָ��
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
	//��ʽ���
	Win32KeyControlInfo* setCombinKey(int vkey=-1);

	bool getAsHoldKey(int *vkey, int *keyType);
	void setAsHoldKey(int vkey,int keyType);

	bool getAsKey(int *vkey);
	void setAsKey(int vkey);

	//����ֵΪ����ĳ��ȣ�����'\0'
	//TODO:ʵ������������
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
