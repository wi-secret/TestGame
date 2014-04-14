#include "MyKeyboardControl.h"

MyKeyboardControl* MyKeyboardControl::theInstance = NULL;

MyKeyboardControl::MyKeyboardControl()
{
	m_bisActive = false;
}


MyKeyboardControl::~MyKeyboardControl()
{
	clearKeyMap();
}

bool MyKeyboardControl::isActive() {
	return m_bisActive;
}

void MyKeyboardControl::pushKeyCallback(int keyValue, onKeyDown inFunc,void *userdata) {
	map<char, list < pair< onKeyDown , void*> >*>::iterator mit=keyMap.find(keyValue);
	if (mit == keyMap.end() || mit->second == NULL) {
		list<pair < onKeyDown,void* > >* lk = new list<pair< onKeyDown,void*> >;
		lk->push_back(pair<onKeyDown,void*>(inFunc,userdata));
		keyMap[keyValue] = lk;
		return;
	}
	mit->second->push_back(pair<onKeyDown,void*>(inFunc,userdata));
}

void MyKeyboardControl::popKeyCallback(int keyValue) {
	map<char, list < pair< onKeyDown, void*> >*>::iterator mit = keyMap.find(keyValue);
	if (mit == keyMap.end() || mit->second == NULL) {
		return;
	}
	mit->second->pop_back();
}

void MyKeyboardControl::clearKeyMap() {
	map<char, list < pair< onKeyDown, void*> >*>::iterator mend = keyMap.end();
	for (map<char, list < pair< onKeyDown, void*> >*>::iterator mit = keyMap.begin(); mit != mend; mit++) {
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

void MyKeyboardControl::checkKeyState() {
	map<char, list < pair< onKeyDown, void*> >*>::iterator mend = keyMap.end();
	for (map<char, list < pair< onKeyDown, void*> >*>::iterator mit = keyMap.begin(); mit != mend; mit++) {
		if (GetAsyncKeyState(mit->first) & 0x8000) {
			list< pair< onKeyDown,void*> >::reverse_iterator lrend = mit->second->rend();
			for (list< pair<onKeyDown,void*> >::reverse_iterator lrit = mit->second->rbegin(); lrit != lrend; lrit++) {
				if (lrit->first(lrit->second)) {
					break;
				}
			}
		}
	}
}

void MyKeyboardControl::setActivation(bool in) {
	m_bisActive = in;
}