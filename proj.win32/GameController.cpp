#include "GameController.h"

template <typename T> MyGameController* GameController::theInstance = NULL;

template <typename T> MyGameController* GameController::getInstance() {
	if (theInstance == NULL) {
		theInstance = new MyGameController();
	}
	return theInstance;
}

template <typename T> GameController::GameController()
{
	activation=false;
}

template <typename T> void GameController::release() {
	dislinkHardwareKey();
}

template <typename T> void GameController::linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata){
	cbFuncMaps[crntScene][vLogicKey].first = eventFunc;
	cbFuncMaps[crntScene][vLogicKey].second = userdata;
}

template <typename T> void GameController::changeSceneTo(int sceneCode) {
	dislinkHardwareKey();
	crntScene = sceneCode;
	linkHardwareKey();
}

template <typename T> void GameController::setToDefault() {
	for(map<int,map<int,T>>::iterator i=defaultKeyMap.begin();i!=defualtKeyMap.end();i++) {
		changeSceneTo(i->first);
		for(map<int,T>::iterator j=i->second.begin();j!=i->second.end();j++) {
			setLogicKeyInfo(j->first,j->second);
		}
	}
}

template <typename T> void GameController::repairKeyConfig() {
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

template <typename T> void GameController::enable(bool in) {
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
	TiXmlDocument doc(configFileStorage);
	if (!doc.LoadFile()) {
		TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
		doc.LinkEndChild(decl);
	}
	TiXmlElement *pEle=doc.FirstChildElement("controlSetting");
	if (pEle == NULL) {
		pEle = new TiXmlElement("controlSetting");
		doc.LinkEndChild(pEle);
	}
	TiXmlElement *pnEle = pEle->FirstChildElement("win32Keyboard");
	if (pnEle == NULL) {
		pnEle = new TiXmlElement("win32Keyboard");
		pEle->LinkEndChild(pnEle);
	}
	pEle = pnEle;
	pEle->Clear();
	for (map<int, map<int, int> >::iterator it = hardwareKeyMaps.begin(); it != hardwareKeyMaps.end(); it++) {
		for (map<int, int>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			pnEle = new TiXmlElement("keySetting");
			pnEle->SetAttribute("sceneCode", it->first);
			pnEle->SetAttribute("logicKey", it2->first);
			pnEle->SetAttribute("virtualKey", it2->second);
		}
	}
	return doc.SaveFile();
}

bool Win32KeyboardController::loadConfig() {
	bool res=true;
	TiXmlDocument doc(configFileStorage);
	if (!doc.LoadFile()) {
		return false;
	}
	TiXmlElement *pEle = doc.FirstChildElement("controlSetting");
	if (pEle == NULL) {
		return false;
	}
	pEle = pEle->FirstChildElement("win32Keyboard");
	if (pEle == NULL) {
		return false;
	}
	if (pEle->FirstChild("keySetting") == NULL) {
		return false;
	}
	for (TiXmlElement *it = pEle->FirstChildElement("keySetting");
		it != pEle->LastChild("keySetting")->ToElement();
		it = it->NextSibling()->ToElement()) {
		int logicKey, virtualKey,scene;
		if (TIXML_SUCCESS != it->QueryIntAttribute("logicKey", &logicKey)) {
			continue;
			res = false;
		}
		if (TIXML_SUCCESS != it->QueryIntAttribute("virtualKey", &virtualKey)) {
			continue;
			res = false;
		}
		if (TIXML_SUCCESS != it->QueryIntAttribute("sceneCode", &scene)) {
			continue;
			res = false;
		}
		hardwareKeyMaps[scene][logicKey] = virtualKey;
	}
	if (pEle->LastChild() != NULL) {
		int logicKey, virtualKey, scene;
		if (TIXML_SUCCESS != pEle->LastChild()->ToElement()->QueryIntAttribute("logicKey", &logicKey)) {
			return false;
		}
		if (TIXML_SUCCESS != pEle->LastChild()->ToElement()->QueryIntAttribute("virtualKey", &virtualKey)) {
			return false;
		}
		if (TIXML_SUCCESS != pEle->LastChild()->ToElement()->QueryIntAttribute("sceneCode", &scene)) {
			return false;
		}
		hardwareKeyMaps[scene][logicKey] = virtualKey;
	}
	return res;
}

void Win32KeyboardController::dislinkHardwareKey() {
	MyKeyboardControl::getInstance()->clearAllKeyMap();
}

//TODO:从这里开始继续实现
void Win32KeyboardController::linkHardwareKey() {
	dislinkHardwareKey();
}

bool Win32KeyboardController::cbGeneralOnKeyDown(void* vLogicKey) {
	map<int,pair<onButtonDown,void*> >& funcMap=theInstance->cbFuncMaps[theInstance->crntScene];
	map<int, pair<onButtonDown, void*>>::iterator it = funcMap.find(*((int*)vLogicKey));
	if (it == funcMap.end()) {
		return false;
	}
	if (it->second.first == NULL) {
		return false;
	}
	it->second.first(NULL, it->second.second);
	return false;
}


int Win32KeyboardController::getvKey(int scene,int vlogic) {
	return hardwareKeyMaps[scene][vlogic];
}

MyGameController::MyGameController() {
	MyKeyboardControl::getInstance()->setActivation(true);
	defualtKeyMaps[MYGC_SCENE_GAME][MYGC_GAME_LK_MD] = 'S';
	defualtKeyMaps[MYGC_SCENE_GAME][MYGC_GAME_LK_MU] = 'W';
	defualtKeyMaps[MYGC_SCENE_GAME][MYGC_GAME_LK_ML] = 'A';
	defualtKeyMaps[MYGC_SCENE_GAME][MYGC_GAME_LK_MR] = 'D';
	defualtKeyMaps[MYGC_SCENE_GAME][MYGC_GAME_LK_RR] = 'E';
	defualtKeyMaps[MYGC_SCENE_GAME][MYGC_GAME_LK_RL] = 'Q';
}

MyGameController::~MyGameController() {
	release();
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

bool Win32KeyControlInfo::getAsCombinKey(vector<int> *keyorders) {
	if(keytype!=WIN32KEYTYPE_COMBIN) {
		return false;
	}
	vector<int> *_keyorders=(vector<int>*)data;
	keyorders->clear();
	for(vector<int>::iterator i=_keyorders->begin();i!=_keyorders->end();i++) {
		keyorders->push_back(*i);
	}
	return true;
}

void Win32KeyControlInfo::setCombinKey(vector<int> &keyorders) {
	release();
	keytype=WIN32KEYTYPE_COMBIN;
	data=new vector<int>;
	vector<int> *_keyorders=(vector<int>*)data;
	for(vector<int>::iterator i=keyorders.begin();i!=keyorders.end();i++) {
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
