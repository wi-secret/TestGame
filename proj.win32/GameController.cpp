#include "GameController.h"

MyGameController* GameController::theInstance = NULL;

MyGameController* GameController::getInstance() {
	if (theInstance == NULL) {
		theInstance = new MyGameController();
	}
	return theInstance;
}

GameController::GameController()
{
}

void GameController::release() {
	dislinkHardwareKey();
}

void GameController::linkLogicKey(int vLogicKey, onButtonDown eventFunc, void* userdata){
	cbFuncMaps[crntScene][vLogicKey].first = eventFunc;
	cbFuncMaps[crntScene][vLogicKey].second = userdata;
}

void GameController::changeSceneTo(int sceneCode) {
	dislinkHardwareKey();
	crntScene = sceneCode;
	linkHardwareKey();
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
	MyKeyboardControl::getInstance()->clearKeyMap();
	for (vector<int*>::iterator i = vlKey.begin(); i != vlKey.end(); i++) {
		delete *i;
	}
	vlKey.clear();
}

void Win32KeyboardController::linkHardwareKey() {
	dislinkHardwareKey();
	for (map<int, int>::iterator i = hardwareKeyMaps[crntScene].begin(); i != hardwareKeyMaps[crntScene].end(); i++) {
		int *pvLogicKey = new int;
		*pvLogicKey = i->first;
		vlKey.push_back(pvLogicKey);
		MyKeyboardControl::getInstance()->pushKeyCallback(i->second, cbGeneralOnKeyDown, pvLogicKey);
	}
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

void Win32KeyboardController::repairKeyConfig() {
	for (map<int, map<int, int>>::iterator it = defualtKeyMaps.begin(); it != defualtKeyMaps.end(); it++) {
		map<int, map<int,int>>::iterator iscene = hardwareKeyMaps.find(it->first);
		if (iscene == hardwareKeyMaps.end()) {
			hardwareKeyMaps[it->first];
			iscene = hardwareKeyMaps.find(it->first);
		}
		for (map<int, int>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if (iscene->second.find(it2->first) == iscene->second.end()) {
				iscene->second[it2->first] = it2->second;
			}
		}
	}
}

void Win32KeyboardController::setToDefault() {
	hardwareKeyMaps = defualtKeyMaps;
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