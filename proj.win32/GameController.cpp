#include "GameController.h"

template <typename T> GameController<T>::GameController()
{
	activation=false;
}

template <typename T> void GameController<T>::release() {
	dislinkHardwareKey();
}

template <typename T> void GameController<T>::linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata){
	cbFuncMaps[crntScene][vLogicKey].first = eventFunc;
	cbFuncMaps[crntScene][vLogicKey].second = userdata;
}

template <typename T> void GameController<T>::changeSceneTo(int sceneCode) {
	dislinkHardwareKey();
	crntScene = sceneCode;
	linkHardwareKey();
}

template <typename T> void GameController<T>::setToDefault() {
	int scene=crntScene;
	for(auto i=defaultKeyMap.begin();i!=defaultKeyMap.end();i++) {
		changeSceneTo(i->first);
		for(map<int,T>::iterator j=i->second.begin();j!=i->second.end();j++) {
			setLogicKeyInfo(j->first,j->second);
		}
	}
	changeSceneTo(scene);
}

template <typename T> void GameController<T>::repairKeyConfig() {
	int scene=crntScene;
	for(map<int,map<int,T>>::iterator i=defualtKeyMap.begin();i!=defualtKeyMap.end();i++) {
		changeSceneTo(i->first);
		for(map<int,T>::iterator j=i->second.begin();j!=i->second.end();j++) {
			T info;
			if(!getLogicKeyInfo(j->first,info)) {
				setLogicKeyInfo(j->first,j->second);
			}
		}
	}
	changeSceneTo(scene);
}

template <typename T> void GameController<T>::enable(bool in) {
	if(in) {
		if(!activation) {
			linkHardwareKey();
			activation=true;
		}
	} else {
		if(activation) {
			dislinkHardwareKey();
			activation=false;
		}
	}
}

template <typename T> bool getActivation() {
	return activation;
}

bool Win32KeyboardController::saveConfig() {
	return true;
}

bool Win32KeyboardController::loadConfig() {
	return true;
}

void Win32KeyboardController::dislinkHardwareKey() {
	MyKeyboardControl::getInstance()->setActivation(false);
	MyKeyboardControl::getInstance()->clearAllKeyMap();
}

void Win32KeyboardController::linkHardwareKey() {
	dislinkHardwareKey();
	for(auto i:hardwareKeyMaps[crntScene]) {
		MyKeyboardControl::getInstance()->pushKeyCallback(
			i.second,
			[this,i] (void* userdata)->bool {
				cbFuncMaps[crntScene][i.first].first(NULL,userdata);
				return true;
			},
			cbFuncMaps[crntScene][i.first].second
		);
	}

	map<int,cbHoldKeyFunc> holdkeylist;
	for(auto i:holdHardwareKeyMap[crntScene]) {
		switch(i.second.second) {
			case KEY_DOWN : {
				holdkeylist[i.second.first].onStart=[this,&i](void* userdata) {
					cbFuncMaps[crntScene][i.first].first(NULL,userdata);
					return true;
				};
				holdkeylist[i.second.first].userdata.start=cbFuncMaps[crntScene][i.first].second;
				break;
			}
			case KEY_HOLD : {
				holdkeylist[i.second.first].onHold=[this,&i](void* userdata) {
					cbFuncMaps[crntScene][i.first].first(NULL,userdata);
					return true;
				};
				holdkeylist[i.second.first].userdata.hold=cbFuncMaps[crntScene][i.first].second;
				break;
			}
			case KEY_RELEASE : {
				holdkeylist[i.second.first].onRelease=[this,&i](void* userdata) {
					cbFuncMaps[crntScene][i.first].first(NULL,userdata);
					return true;
				};
				holdkeylist[i.second.first].userdata.release=cbFuncMaps[crntScene][i.first].second;
				break;
			}
		}
	}
	for(auto i:holdkeylist) {
		MyKeyboardControl::getInstance()->pushHoldKeyCallback(i.first,i.second.onStart,i.second.userdata.start,i.second.onHold,i.second.userdata.hold,i.second.onRelease,i.second.userdata.release);
	}

	for(auto i:combinHardwareKeyMap[crntScene]) {
		MyKeyboardControl::getInstance()->pushCombinKeyCallback(
			i.second,
			[this,&i](void* userdata) {
				cbFuncMaps[crntScene][i.first].first(NULL,userdata);
				return true;
			},
			cbFuncMaps[crntScene][i.first].second
		);
	}

	MyKeyboardControl::getInstance()->setActivation(true);
}

bool Win32KeyboardController::getLogicKeyInfo(int logicKey,Win32KeyControlInfo& info) {
	auto i=hardwareKeyMaps[crntScene].find(logicKey);
	if(i!=hardwareKeyMaps[crntScene].end()) {
		info.setAsKey(i->second);
		return true;
	}
	auto j=holdHardwareKeyMap[crntScene].find(logicKey);
	if(j!=holdHardwareKeyMap[crntScene].end()) {
		info.setAsHoldKey(j->second.first,j->second.second);
		return true;
	}
	auto k=combinHardwareKeyMap[crntScene].find(logicKey);
	if(k!=combinHardwareKeyMap[crntScene].end()) {
		info.setCombinKey(k->second);
		return true;
	}
	return false;
}


void Win32KeyboardController::setLogicKeyInfo(int logicKey,Win32KeyControlInfo& info) {
	switch(info.getType()) {
		case WIN32KEYTYPE_KEY: {
			int key;
			info.getAsKey(&key);
			hardwareKeyMaps[crntScene][logicKey]=key;
			break;
		}
		case WIN32KEYTYPE_HOLD: {
			int key,keystate;
			info.getAsHoldKey(&key,&keystate);
			holdHardwareKeyMap[crntScene][logicKey].first=key;
			holdHardwareKeyMap[crntScene][logicKey].second=keystate;
			break;
		}
		case WIN32KEYTYPE_COMBIN: {
			combinHardwareKeyMap[crntScene][logicKey].clear();
			vector<unsigned char> keyOrders;
			info.getAsCombinKey(&keyOrders);
			for(auto i:keyOrders) {
				combinHardwareKeyMap[crntScene][logicKey].push_back(i);
			}
			break;
		}
	}
}

MyGameController::MyGameController() {
	defaultKeyMap[MYGC_SCENE_GAME][MYGC_GAME_LK_MD].setAsHoldKey('S',WIN32HOLDTYPE_HOLD);
	defaultKeyMap[MYGC_SCENE_GAME][MYGC_GAME_LK_MU].setAsHoldKey('W',WIN32HOLDTYPE_HOLD);
	defaultKeyMap[MYGC_SCENE_GAME][MYGC_GAME_LK_ML].setAsHoldKey('A',WIN32HOLDTYPE_HOLD);
	defaultKeyMap[MYGC_SCENE_GAME][MYGC_GAME_LK_MR].setAsHoldKey('D',WIN32HOLDTYPE_HOLD);
	defaultKeyMap[MYGC_SCENE_GAME][MYGC_GAME_LK_RR].setAsHoldKey('E',WIN32HOLDTYPE_HOLD);
	defaultKeyMap[MYGC_SCENE_GAME][MYGC_GAME_LK_RL].setAsHoldKey('Q',WIN32HOLDTYPE_HOLD);
	setToDefault();
}

MyGameController::~MyGameController() {
	release();
}


MyGameController* MyGameController::theInstance = NULL;

MyGameController* MyGameController::getInstance() {
	if (theInstance == NULL) {
		theInstance = new MyGameController();
	}
	return theInstance;
}


Win32KeyControlInfo::Win32KeyControlInfo() {
	data=NULL;
	keytype=WIN32KEYTYPE_NULL;
}

Win32KeyControlInfo::~Win32KeyControlInfo() {
	release();
}

void Win32KeyControlInfo::release() {
	if(data!=NULL) {
		switch(keytype) {
		case WIN32KEYTYPE_COMBIN:
			delete (vector<int>*)data;
			break;
		case WIN32KEYTYPE_HOLD:
			delete (Win32KeyControlInfo::holdKeyInfo*)data;
			break;
		case WIN32KEYTYPE_KEY:
			delete (int*)data;
			break;
		}
	}
	keytype=WIN32KEYTYPE_NULL;
}

bool Win32KeyControlInfo::getAsCombinKey(vector<unsigned char> *keyorders) {
	if(keytype!=WIN32KEYTYPE_COMBIN) {
		return false;
	}
	vector<int> *_keyorders=(vector<int>*)data;
	keyorders->clear();
	for(auto i=_keyorders->begin();i!=_keyorders->end();i++) {
		keyorders->push_back(*i);
	}
	return true;
}

void Win32KeyControlInfo::setCombinKey(vector<unsigned char> &keyorders) {
	release();
	keytype=WIN32KEYTYPE_COMBIN;
	data=new vector<int>;
	vector<int> *_keyorders=(vector<int>*)data;
	for(auto i=keyorders.begin();i!=keyorders.end();i++) {
		_keyorders->push_back(*i);
	}
}

Win32KeyControlInfo* Win32KeyControlInfo::setCombinKey(int vkey) {
	if(vkey==-1) {
		release();
		keytype=WIN32KEYTYPE_COMBIN;
		data=new vector<int>;
		return this;
	}
	vector<int> *keyorders=(vector<int>*)data;
	keyorders->push_back(vkey);
	return this;
}

bool Win32KeyControlInfo::getAsHoldKey(int *vkey, int *keyType) {
	if(keytype!=WIN32KEYTYPE_HOLD) {
		return false;
	}
	holdKeyInfo *info=(holdKeyInfo*)data;
	*keyType=info->keyState;
	*vkey=info->vkey;
	return true;
}

void Win32KeyControlInfo::setAsHoldKey(int vkey,int keyType) {
	release();
	keytype=WIN32KEYTYPE_HOLD;
	data=new holdKeyInfo;
	holdKeyInfo *info=(holdKeyInfo*)data;
	info->keyState=keyType;
	info->vkey=vkey;
}

bool Win32KeyControlInfo::getAsKey(int* vkey) {
	if(keytype!=WIN32KEYTYPE_KEY) {
		return false;
	}
	*vkey=*((int*)data);
	return true;
}

void Win32KeyControlInfo::setAsKey(int vkey) {
	release();
	keytype=WIN32KEYTYPE_KEY;
	data=new int;
	(*((int*)data))=vkey;
}

int Win32KeyControlInfo::getType() {
	return keytype;
}

int Win32KeyControlInfo::getAsString(char *res) {
	return 0;
}

int Win32KeyControlInfo::getAsString(string &res) {
	return 0;
}
