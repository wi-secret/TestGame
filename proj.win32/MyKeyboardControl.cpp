#include "MyKeyboardControl.h"

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
	map<unsigned char,cbCombinKeyFunc*> *pcrntnxtkeymap;
	if(!keyOrders.empty()) {
		bool precede=true;
		for(int i=0;i<size;i++) {
			if(!(GetAsyncKeyState(vkeyOrders[i]) & 0x8000)) {
				precede=false;
			}
		}
		if(precede) {
			pcrntnxtkeymap=&(keyOrders[size-1]->nextKey);
			precede=false;
			for(map<unsigned char, cbCombinKeyFunc*>::iterator i=pcrntnxtkeymap->begin();i!=pcrntnxtkeymap->end();i++) {
				if(GetAsyncKeyState(i->first) & 0x8000) {
					vkeyOrders.push_back(i->first);
					keyOrders.push_back(i->second);
					precede=true;
					break;
				}
			}
			if(precede) {
				for(vector<cbCombinKeyFunc*>::iterator i=keyOrders.begin();i!=keyOrders.end();i++) {
					(*i)->onHold((*i)->userdata);
				}
				return;
			}
		}
		keyOrders.clear();
		vkeyOrders.clear();
	}

	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=combinKeyMap.begin();i!=combinKeyMap.end();i++) {
		if(GetAsyncKeyState(i->first) & 0x8000) {
			keyOrders.push_back(i->second);
			vkeyOrders.push_back(i->first);
			i->second->onHold(i->second->userdata);
		}
	}
}

void MyKeyboardControl::checkHoldKey() {
	for(map<unsigned char,cbHoldKeyFunc>::iterator i=holdKeyMap.begin();i!=holdKeyMap.end();i++) {
		int statcode=GetAsyncKeyState(i->first);
		if(statcode==1) {
			i->second.onRelease(i->second.userdata);
		} else if(statcode==0x8001) {
			i->second.onHold(i->second.userdata);
		} else if(statcode==0x8000) {
			i->second.onStart(i->second.userdata);
		}
	}
}

void MyKeyboardControl::checkKeyState() {
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

void MyKeyboardControl::setActivation(bool in) {
	m_bisActive = in;
}

void MyKeyboardControl::pushHoldKeyCallback(int keyValue,onKeyDown onStart,onKeyDown onHold,onKeyDown onRelease,void *userdata) {
	cbHoldKeyFunc holdkey;
	holdkey.onHold=onHold;
	holdkey.onStart=onStart;
	holdkey.onRelease=onRelease;
	holdkey.userdata=userdata;
	holdKeyMap[keyValue]=holdkey;
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
		crntKeyPos->onHold=NULL;
		crntKeyPos->userdata=NULL;
		combinKeyMap[keyOrder[0]]=crntKeyPos;
	} else {
		crntKeyPos=firstKey->second;
	}
	for(vector<unsigned char>::iterator iko=keyOrder.begin()+1;iko!=keyOrder.end();iko++) {
		map<unsigned char,cbCombinKeyFunc*>::iterator inxtk=crntKeyPos->nextKey.find(*iko);
		if(inxtk==crntKeyPos->nextKey.end()) {
			cbCombinKeyFunc *newcbfunc=new cbCombinKeyFunc;
			newcbfunc->onHold=NULL;
			newcbfunc->userdata=NULL;
			crntKeyPos->nextKey[*iko]=newcbfunc;
			crntKeyPos=newcbfunc;
		} else {
			crntKeyPos=inxtk->second;
		}
	}
	crntKeyPos->onHold=cbFunc;
	crntKeyPos->userdata=userdata;
}

MyKeyboardControl* MyKeyboardControl::addCombinKey(unsigned char key,onKeyDown cbFunc,void *userdata) {
	if(key==0) {
		crntAddPos=NULL;
	}
	map<unsigned char, cbCombinKeyFunc*> *pcrntnxtmap=NULL;
	if(crntAddPos==NULL) {
		pcrntnxtmap=&combinKeyMap;
	} else {
		pcrntnxtmap=&(crntAddPos->nextKey);
	}
	map<unsigned char, cbCombinKeyFunc*>::iterator inxtk=pcrntnxtmap->find(key);
	if(inxtk==pcrntnxtmap->end()) {
		crntAddPos=new cbCombinKeyFunc;
		crntAddPos->onHold=NULL;
		crntAddPos->userdata=NULL;
		pcrntnxtmap->operator[](key)=crntAddPos;
	} else {
		crntAddPos=inxtk->second;
	}
	if(cbFunc!=NULL&&cbFunc!=MyKeyboardControl::clear_combin_key_func) {
		crntAddPos->onHold=cbFunc;
		crntAddPos->userdata=userdata;
	} else if(cbFunc==MyKeyboardControl::clear_combin_key_func) {
		crntAddPos->onHold=NULL;
		crntAddPos->userdata=NULL;
	}
	return this;
}

bool MyKeyboardControl::clear_combin_key_func(void*) {
	return false;
}

void MyKeyboardControl::clearCombinKeyMap() {
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=combinKeyMap.begin();i!=combinKeyMap.end();i++) {
		deleteCombinKeyMap(i->second);
	}
}
void MyKeyboardControl::deleteCombinKeyMap(cbCombinKeyFunc* startpos) {
	for(map<unsigned char,cbCombinKeyFunc*>::iterator i=startpos->nextKey.begin();i!=startpos->nextKey.end();i++) {
		deleteCombinKeyMap(i->second);
	}
	delete startpos;
}