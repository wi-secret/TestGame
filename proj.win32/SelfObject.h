#pragma once
#include "MovableObject.h"


class SelfObject : public MovableObject{
public:
	static SelfObject* create();
	static SelfObject* create(const char* image);
};