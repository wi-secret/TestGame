#include "e_speedlimit.h"


void e_speedlimit::execute() {
	MovableObject *par = (MovableObject*)GetParent();
	par->speed = ccp(0, 0);
}

void e_speedlimit::onStart() {
}

bool e_speedlimit::isEnd() {
	return false;
}

e_speedlimit::~e_speedlimit() {
	if (backupCopy != NULL) {
		((e_speedlimit*)backupCopy)->backupCopy = NULL;
		delete (e_speedlimit*)backupCopy;
		backupCopy = NULL;
	}
}
