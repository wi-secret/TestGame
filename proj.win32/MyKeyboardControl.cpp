#include "MyKeyboardControl.h"
#pragma once
MyKeyboardControl* MyKeyboardControl::theInstance = NULL;

MyKeyboardControl::MyKeyboardControl()
{
	m_bisActive = false;
}


MyKeyboardControl::~MyKeyboardControl()
{
	clearKeyMap();
	clearHoldKeyMap();
	clearCombinKeyMap();
}

bool MyKeyboardControl::isActive() {
	return m_bisActive;
}

void MyKeyboardControl::pushKeyCallback(int keyValue, onKeyDown inFunc,void *userdata) {
	map<unsigned char, list < pair< onKeyDown , void*> >*>::iterator mit=keyMap.find(keyValue);
	if (mit == keyMap.end() || mit->second == NULL) {
		list<pair < onKeyDown,void* > >* lk = new list<pair< onKeyDown,void*> >;
		lk->push_back(pair<onKeyDown,void*>(inFunc,userdata));
		keyMap[keyValue] = lk;
		return;
	}
	mit->second->push_back(pair<onKeyDown,void*>(inFunc,userdata));
	keysStat[keyValue]=0;
}

void MyKeyboardControl::popKeyCallback(int keyValue) {
	map<unsigned char, list < pair< onKeyDown, void*> >*>::iterator mit = keyMap.find(keyValue);
	if (mit == keyMap.end() || mit->second == NULL) {
		return;
	}
	mit->second->pop_back();
}

void MyKeyboardControl::clearKeyMap() {
	map<unsigned char, list < pair< onKeyDown, void*> >*>::iterator mend = keyMap.end();
	for (map<unsigned char, list < pair< onKeyDown, void*> >*>::iterator mit = keyMap.begin(); mit != mend; mit++) {
		if (mit->second == NULL) {
			continue;
		}
		delete mit->second;
		mit->second = NULL;
	}
}

MyKeyboardControl* MyKeyboardControl::getInstance() {
	if (theInstance == NULL) {
		theInstance = new MyKeyboardControl();
	}
	return theInstance;
}

void MyKeyboardControl::release() {
	delete theInstance;
}

void MyKeyboardControl::checkCombinKey() {
	if(keyOrders.size()!=vkeyOrders.size()) {
		keyOrders.clear();
		vkeyOrders.clear();
	}
	int size=keyOrders.size();
	for(int i=0;i<size;i++) {
		if(keysStat[vkeyOrders[i]]!=KEY_HOLD) {
			for(int j=i;j<size;j++) {
				vkeyOrders.pop_back();
				keyOrders.pop_back();
			}
			break;
		}
		keyOrders[i]->onHold(keyOrders[i]->userdata);
	}
	size=keyOrders.size();
	map<unsigned char,cbCombinKeyFunc*> *pcrntnxtkeymap=NULL;
	if(!keyOrders.empty()) {
		pcrntnxtkeymap=&(keyOrders[size-1]->nextKey);
	} else {
		pcrntnxtkeymap=&combinKeyMap;
	}
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=pcrntnxtkeymap->begin();i!=pcrntnxtkeymap->end();i++) {
		if(keysStat[i->first]==KEY_DOWN) {
			keyOrders.push_back(i->second);
			vkeyOrders.push_back(i->first);
			i->second->onHold(i->second->userdata);
		}
	}
}

void MyKeyboardControl::checkHoldKey() {
	for(map<unsigned char,cbHoldKeyFunc>::iterator i=holdKeyMap.begin();i!=holdKeyMap.end();i++) {
		if(keysStat[i->first]==KEY_DOWN) {
			i->second.onStart(i->second.userdata.start);
		} else if(keysStat[i->first]==KEY_HOLD) {
			i->second.onHold(i->second.userdata.hold);
		} else if(keysStat[i->first]==KEY_RELEASE) {
			i->second.onRelease(i->second.userdata.release);
		}
	}
}

void MyKeyboardControl::resetState() {
	keyOrders.clear();
	keysStat.clear();
	vkeyOrders.clear();
	for(map<unsigned char, list < pair <onKeyDown , void*> >*>::iterator i=keyMap.begin();i!=keyMap.end();i++) {
		keysStat[i->first]=KEY_IDLE;
	}
	for(map<unsigned char, cbHoldKeyFunc>::iterator i=holdKeyMap.begin();i!=holdKeyMap.end();i++) {
		keysStat[i->first]=KEY_IDLE;
	}
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=combinKeyMap.begin();i!=combinKeyMap.end();i++) {
		keysStat[i->first]=KEY_IDLE;
		resetCombinKeyState(i->second);
	}
}

void MyKeyboardControl::resetCombinKeyState(cbCombinKeyFunc *pcrnt) {
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=pcrnt->nextKey.begin();i!=pcrnt->nextKey.end();i++) {
		keysStat[i->first]=KEY_IDLE;
		resetCombinKeyState(i->second);
	}
}

void MyKeyboardControl::checkKeyState() {
	if(m_bisActive) {
		for(map<unsigned char,int>::iterator i=keysStat.begin();i!=keysStat.end();i++) {
			if(GetAsyncKeyState(i->first) & 0x8000) {
				if(i->second==KEY_HOLD||i->second==KEY_DOWN) {
					i->second=KEY_HOLD;
				} else {
					i->second=KEY_DOWN;
				}
			} else {
				if(i->second==KEY_HOLD||i->second==KEY_DOWN) {
					i->second=KEY_RELEASE;
				} else {
					i->second=KEY_IDLE;
				}
			}
		}
		map<unsigned char, list < pair< onKeyDown, void*> >*>::iterator mend = keyMap.end();
		for (map<unsigned char, list < pair< onKeyDown, void*> >*>::iterator mit = keyMap.begin(); mit != mend; mit++) {
			if (GetAsyncKeyState(mit->first) & 0x8000) {
				list< pair< onKeyDown,void*> >::reverse_iterator lrend = mit->second->rend();
				for (list< pair<onKeyDown,void*> >::reverse_iterator lrit = mit->second->rbegin(); lrit != lrend; lrit++) {
					if (lrit->first(lrit->second)) {
						break;
					}
				}
			}
		}
		checkCombinKey();
		checkHoldKey();
	}
}

void MyKeyboardControl::setActivation(bool in) {
	if(!m_bisActive&&in) {
		resetState();
	}
	m_bisActive = in;
}

void MyKeyboardControl::pushHoldKeyCallback(int keyValue,onKeyDown onStart,void* startUserdata,onKeyDown onHold,void* holdUserdata,onKeyDown onRelease,void *releaseUserdata) {
	cbHoldKeyFunc holdkey;
	holdkey.onHold=onHold;
	holdkey.onStart=onStart;
	holdkey.onRelease=onRelease;
	holdkey.userdata.hold=holdUserdata;
	holdkey.userdata.start=startUserdata;
	holdkey.userdata.release=releaseUserdata;
	holdKeyMap[keyValue]=holdkey;
	keysStat[keyValue]=0;
}

void MyKeyboardControl::popHoldKeyCallback(int keyValue) {
	holdKeyMap.erase(keyValue);
}

void MyKeyboardControl::clearHoldKeyMap() {
	holdKeyMap.clear();
}

void MyKeyboardControl::pushCombinKeyCallback(vector<unsigned char>& keyOrder,onKeyDown cbFunc,void* userdata) {
	if(keyOrder.empty()) {
		return;
	}
	cbCombinKeyFunc *crntKeyPos=NULL;
	map<unsigned char,cbCombinKeyFunc*>::iterator firstKey=combinKeyMap.find(keyOrder[0]);
	if(firstKey==combinKeyMap.end()) {
		crntKeyPos=new cbCombinKeyFunc;
		combinKeyMap[keyOrder[0]]=crntKeyPos;
		keysStat[keyOrder[0]]=0;
	} else {
		crntKeyPos=firstKey->second;
	}
	for(vector<unsigned char>::iterator iko=keyOrder.begin()+1;iko!=keyOrder.end();iko++) {
		map<unsigned char,cbCombinKeyFunc*>::iterator inxtk=crntKeyPos->nextKey.find(*iko);
		if(inxtk==crntKeyPos->nextKey.end()) {
			cbCombinKeyFunc *newcbfunc=new cbCombinKeyFunc;
			crntKeyPos->nextKey[*iko]=newcbfunc;
			crntKeyPos=newcbfunc;
			keysStat[*iko]=0;
		} else {
			crntKeyPos=inxtk->second;
		}
	}
	crntKeyPos->onHold=cbFunc;
	crntKeyPos->userdata=userdata;
}


MyKeyboardControl* MyKeyboardControl::addCombinKey() {
	crntAddPos=NULL;
	return this;
}

MyKeyboardControl* MyKeyboardControl::addCombinKey(unsigned char key,int operate_code) {
	map<unsigned char, cbCombinKeyFunc*> *pcrntnxtmap=NULL;
	if(crntAddPos==NULL) {
		pcrntnxtmap=&combinKeyMap;
	} else {
		pcrntnxtmap=&(crntAddPos->nextKey);
	}
	map<unsigned char, cbCombinKeyFunc*>::iterator inxtk=pcrntnxtmap->find(key);
	if(inxtk==pcrntnxtmap->end()) {
		crntAddPos=new cbCombinKeyFunc;
		pcrntnxtmap->operator[](key)=crntAddPos;
	} else {
		crntAddPos=inxtk->second;
	}
	switch(operate_code) {
	case ADD_COMBIN_VIA: {
		break;
	}
	case ADD_COMBIN_CLEAR: {
		crntAddPos->onHold=[](void*){ return false; };
		crntAddPos->userdata=nullptr;
		break;
	}
	}
	return this;
}

MyKeyboardControl* MyKeyboardControl::addCombinKey(unsigned char key,onKeyDown cbFunc,void *userdata) {
	map<unsigned char, cbCombinKeyFunc*> *pcrntnxtmap=NULL;
	if(crntAddPos==NULL) {
		pcrntnxtmap=&combinKeyMap;
	} else {
		pcrntnxtmap=&(crntAddPos->nextKey);
	}
	map<unsigned char, cbCombinKeyFunc*>::iterator inxtk=pcrntnxtmap->find(key);
	if(inxtk==pcrntnxtmap->end()) {
		crntAddPos=new cbCombinKeyFunc;
		pcrntnxtmap->operator[](key)=crntAddPos;
	} else {
		crntAddPos=inxtk->second;
	}
	crntAddPos->onHold=cbFunc;
	crntAddPos->userdata=userdata;
	keysStat[key]=0;
	return this;
}


void MyKeyboardControl::clearCombinKeyMap() {
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=combinKeyMap.begin();i!=combinKeyMap.end();i++) {
		deleteCombinKeyMap(i->second);
	}
	combinKeyMap.clear();
}
void MyKeyboardControl::deleteCombinKeyMap(cbCombinKeyFunc* startpos) {
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=startpos->nextKey.begin();i!=startpos->nextKey.end();i++) {
		deleteCombinKeyMap(i->second);
	}
	delete startpos;
}

void MyKeyboardControl::clearAllKeyMap() {
	clearCombinKeyMap();
	clearHoldKeyMap();
	clearKeyMap();
}