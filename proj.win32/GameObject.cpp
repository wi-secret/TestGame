#include "GameObject.h"


GameObject::GameObject(void)
{
	maxTag=0;
}

GameObject::~GameObject(void)
{
	for(list<Effect*>::iterator i=pEffectArray.begin();i!=pEffectArray.end();i++) {
		delete (*i);
	}
}

int GameObject::addEffect(Effect* effect) {
	pEffectArray.push_back(effect);
	effect->tag=maxTag;
	effect->parent=this;
	(effect->*(effect->__onStart))();
	effect->backup();
	maxTag++;
	pEffectArray.sort(GameObject::effectPrioritySort);
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