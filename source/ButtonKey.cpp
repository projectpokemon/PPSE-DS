/*
 * ButtonKey.cpp
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include <ulx.h>
#include "Widget.h"
#include "Label.h"
#include "Button.h"
#include "ButtonKey.h"

//void ButtonKey::_press() {
//	if(!_canPress) return;
//	pressCounter = highlightTime;
//	isInvalidated = true;
//}

ButtonKey::ButtonKey(int x, int y, int w, int h, const char *str) : Button(x,y,w,h,str) {
	keyfunct = NULL;
	keyFunctionoid = NULL;
}

ButtonKey::~ButtonKey() {
	if(keyFunctionoid != NULL)
		delete keyFunctionoid;
}

void ButtonKey::doUpdate() {
	if(doPress) {
		if(function != NULL) (*function)();
		if(functionoid != NULL) functionoid->execute();
		if(keyfunct != NULL) (*keyfunct)((const char*)_str);
		if(keyFunctionoid != NULL) keyFunctionoid->execute((const char*)_str);
		doPress = false;
	}
	//Widget::doUpdate();
}
