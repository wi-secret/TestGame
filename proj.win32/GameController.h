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
	//��õ�������ָ��
	static MyGameController* getInstance();
	
	//��ӻ��޸Ķ�Ӧ�߼���������Ӧ����
	virtual void linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata);
	//���水�����ã���ʵ�ʰ������߼�������ӳ�䣩
	virtual bool saveConfig()=0;
	//���밴���趨
	virtual bool loadConfig()=0;
	//�������趨����ΪĬ��
	virtual void setToDefault() = 0;
	//�޸������趨������û��ʵ�ʰ�����Ե��߼�������ΪĬ�ϰ���
	virtual void repairKeyConfig() = 0;
	//���İ�������
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
	//��ö�Ӧ�߼��������������ֵ
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