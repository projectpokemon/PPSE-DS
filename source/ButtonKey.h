/*
 * ButtonKey.h
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */

#ifndef BUTTONKEY_H_
#define BUTTONKEY_H_

#include "Funct.h"

class ButtonKey: public Button {
protected:
	FunctKeyPtr keyFunctionoid;
	void (*keyfunct)(const char*);
	//virtual void _press();
public:
	ButtonKey(int x, int y, int w, int h, const char *str);
	virtual ~ButtonKey();
	virtual inline void setKeyFunction(void (*funPtr)(const char*)) { keyfunct = funPtr; }
	virtual inline void setKeyFunctionoid(FunctKeyPtr fun) { keyFunctionoid = fun; }
	virtual void doUpdate();
	//functionoid
};

#endif /* BUTTONKEY_H_ */
