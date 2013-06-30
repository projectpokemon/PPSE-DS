/*
 * Keypad.cpp
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include <cctype>
#include "Toolkit.h"
#include <ulx.h>
#include "Keypad.h"

//these are only of single keys, the oddly sized keys need to be added
//manually to the board, but also we need to define row length and the
//start offset of each row in another array
const char * lKeys[] = { "`1234567890-=", "qwertyuiop[]\\", "asdfghjkl;'",  "zxcvbnm,./" };
const char * uKeys[] = { "~!@#$%^&*()_+", "QWERTYUIOP{}|",  "ASDFGHJKL:\"", "ZXCVBNM<>?" };
const s8 kLength[] = {13,13,11,10};
const s8 kStart[] = {0, 16+8, 16+10, 16+16};
const int numKeys = 47;

class KeyPadFunct : public FunctKey {
	Keypad *p;
	public:
	KeyPadFunct(Keypad *n) { p = n; }
	inline void execute(const char* str) {
		if(p->_isShift && !p->_isCapLock) {
			//p->upper->disable();
			//p->lower->enable();
			p->_unsetShift = true;
			//p->_isShift = false;
		}
		p->kpress(str);
		//p->isInvalidated = true;
	}
};

class KeyPadSpecialFunct : public Funct {
	Keypad *p;
	char * _str;
	public:
	KeyPadSpecialFunct(Keypad *n, const char * str) {
		p = n;
		_str = (char*)malloc(strlen(str));
		strcpy(_str,str);
		p->isInvalidated = true;
	}
	virtual ~KeyPadSpecialFunct() {
		free(_str);
	}
	inline void execute() {
		if(p->_isShift && !p->_isCapLock) {
			//p->upper->disable();
			//p->lower->enable();
			p->_unsetShift = true;
			//p->_isShift = false;
		}
		p->kpress(_str);
	}
};

class KeyPadShiftFunct : public Funct {
	Keypad *p;
	bool _isLock;
	public:
	KeyPadShiftFunct(Keypad *n, bool lock) { p = n; _isLock = lock; }
	inline void execute() {
		p->_isCapLock = false;
		p->_isShift = !p->_isShift;
		if(p->_isShift && _isLock) p->_isCapLock = true;

		if(p->_isShift) {
			p->upper->enable();
			p->lower->disable();
		} else {
			p->upper->disable();
			p->lower->enable();
		}

		//p->isInvalidated = true;
	}
};

void Keypad::kpress(const char * str) {
	if(retFunoid != NULL) retFunoid->execute(str);
	if(retFun != NULL) retFun(str);
}

Keypad::Keypad(int x, int y)  : Widget(x,y,240,80) {
	retFunoid = NULL;
	retFun = NULL;

	int w = 16;
	int h = 16;

	_isShift = false;
	_isCapLock = false;
	_unsetShift = false;

	//lower case
	lower = new Widget(0,0,240,80);
	upper = new Widget(0,0,240,80);
	//lower = new Widget(x,y,240,80);
	//upper = new Widget(x,y,240,80);

	upper->disable();

	//int nw = w-1;
	//int nh = h-1;
	int nw = w;
	int nh = h;

	{
		char tmp[4];
		ButtonKey *b = NULL;
		KeyPadFunct *kf = new KeyPadFunct(this);
		for(int i=0; i<4; ++i) {
			for(int j=0; j<kLength[i]; ++j) {
				memset(tmp,0,4);
				strncpy(tmp,lKeys[i]+j,1);
				b = new ButtonKey(kStart[i]+j*nw,i*nh,w,h,tmp);
				b->setKeyFunctionoid(kf);
				b->setColor(0);
				//b->setDrawBorder(false);
				lower->addChild(b);

				memset(tmp,0,4);
				strncpy(tmp,uKeys[i]+j,1);
				b = new ButtonKey(kStart[i]+j*nw,i*nh,w,h,tmp);
				b->setKeyFunctionoid(kf);
				b->setColor(0);
				//b->setDrawBorder(false);
				upper->addChild(b);
			}
		}
	}
	Button *b = NULL;
	b = new Button(kStart[0]+13*nw,0,w+8,h,"<-");
	b->setFunctionoid(new KeyPadSpecialFunct(this,"\b"));
	b->setColor(0);
	addChild(b);
	b = new Button(0,1*nh,w+8,h,"TAB");
	b->setFunctionoid(new KeyPadSpecialFunct(this,"\t"));
	b->setColor(0);
	addChild(b);
	b = new Button(0,2*nh,w+10,h,"CAP");
	b->setFunctionoid(new KeyPadShiftFunct(this,true));
	b->setColor(0);
	addChild(b);
	b = new Button(kStart[2]+11*nw,2*nh,w+14,h,"RET");
	b->setFunctionoid(new KeyPadSpecialFunct(this,"\r"));
	b->setColor(0);
	addChild(b);

	b = new Button(0,3*nh,w*2,h,"SHFT");
	b->setFunctionoid(new KeyPadShiftFunct(this,false));
	b->setColor(0);
	addChild(b);
	b = new Button(kStart[3]+10*nw,3*nh,w*2+8,h,"SHFT");
	b->setFunctionoid(new KeyPadShiftFunct(this,false));
	b->setColor(0);
	addChild(b);

	b = new Button(nw*4-4,4*nh,88,h," ");
	b->setFunctionoid(new KeyPadSpecialFunct(this," "));
	b->setColor(0);
	addChild(b);


	addChild(upper);
	addChild(lower);

}

Keypad::~Keypad() {
	if(retFunoid != NULL) delete retFunoid;
	delete upper;
	delete lower;
}

//
bool Keypad::checkInvalidated() {
	if(Widget::checkInvalidated()) return true;
	if(_unsetShift) {
		_isShift = false;
		_unsetShift = false;
		upper->disable();
		lower->enable();
		return true;
	}
	return false;
}
