/*
 * Label.cpp
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include <cctype>
#include "Toolkit.h"
#include <ulx.h>
#include "Numpad.h"

class NumPadFunct : public FunctKey {
	Numpad *p;
	public:
	NumPadFunct(Numpad *n) { p = n; }
	inline void execute(const char* str) { p->kpress(str); }
};

void Numpad::kpress(const char * str) {
	int i = -1;
	if (isdigit(str[0])) i = atoi(str);
	if(retFunoid != NULL) retFunoid->execute(i);
	if(retFun != NULL) retFun((s16)i);
}

Numpad::Numpad(int x, int y, int w, int h)  : Widget(x,y,w*3+h*2,w*4+h*3) {
	retFunoid = NULL;
	retFun = NULL;

	char *cnum = (char*)"789456123";
	char tmp[4];
	ButtonKey *b = NULL;
	NumPadFunct *kf = new NumPadFunct(this);

	for(int i=0; i<9; ++i) {
		memset(tmp,0,4);
		strncpy(tmp,cnum+i,1);
		b = new ButtonKey((i%3)*(w+h),(i/3)*(w+h),w,w,tmp);
		b->setKeyFunctionoid(kf);
		b->setColor(0);
		addChild(b);
	}

	//add zero
	b = new ButtonKey(0,(w+h)*3,w*2+h,w,"0");
	b->setKeyFunctionoid(kf);
	b->setColor(0);
	addChild(b);

	//add backspace
	b = new ButtonKey(w*2+h*2,(w+h)*3,w,w,"<-");
	b->setKeyFunctionoid(kf);
	b->setColor(0);
	addChild(b);
}

Numpad::~Numpad() {
	if(retFunoid != NULL) delete retFunoid;
}
