#include "GameObject.h"


GameObject::GameObject(void)
{
	maxTag = 0;
	isDestroyed = 0;
	ObjectType = TEST_BASIC_OBJ;
}

GameObject::~GameObject(void)
{
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		delete (*i);
	}
}

int GameObject::addEffect(Effect* effect) {
	pEffectAddArray.push_back(effect);
	effect->tag=maxTag;
	effect->parent=this;
	maxTag++;
	return maxTag-1;
}

Effect* GameObject::getEffect(int effectID) {
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if((*i)->tag==effectID) {
			return *i;
		}
	}
	return NULL;
}

bool GameObject::removeEffect(int effectID) {
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if((*i)->tag==effectID) {
			delete (*i);
			pEffectArray.erase(i);
			return true;
		}
	}
	return false;
}

void GameObject::foreachEffect(forEffect f,void* userdata) {
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if(!f(*i,(*i)->tag,userdata)) {
			return;
		}
	}
}

void GameObject::AI() {
	list<int> removeList;
	for(list<Effect*>::iterator i=pEffectAddArray.begin();i!=pEffectAddArray.end();i++) {
		((*i)->*((*i)->__onStart))();
		(*i)->backup();
		pEffectArray.push_back(*i);
	}
	pEffectAddArray.clear();
	pEffectArray.sort();
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		if(((*i)->*((*i)->__isEnd))()) {
			removeList.push_back((*i)->tag);
		} else {
			((*i)->*((*i)->__execute))();
		}
	}
	for(list<int>::iterator i=removeList.begin();i!=removeList.end();i++) {
		removeEffect(*i);
	}
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		(*i)->recovery();
	}
}
bool GameObject::effectPrioritySort(Effect *x,Effect *y) {
		return x->priority>y->priority;
}