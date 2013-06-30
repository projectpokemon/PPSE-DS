/*
 * Button.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include "Widget.h"
#include <ulx.h>
#include "Label.h"
#include "Button.h"

void Button::_press() {
	if(!_canPress) return;
	pressCounter = highlightTime;
	isInvalidated = true;
	doPress = true;
}

Button::Button(int x, int y, int w, int h, const char *str) : Label(x,y,w,h,str) {
	function = NULL;
	functionoid = NULL;

	highlightTime = buttonHighlightTime;
	//isPressed = false;
	_keyPress = 0;
	pressCounter = 0;
	_borderSet = true;
	_backgroundSet = true;
	_border = buttonBorderColor;
	_canPress = true;

	doPress = false;
}

Button::~Button() {
	if(functionoid != NULL)
		delete functionoid;
}

void Button::draw(int x, int y) {
	if(!isVisible) return;
	//_border = labelBorderColor;
	//_background = labelBackgroundColor;
	rgb bg = buttonInactiveBGColor;
	if(pressCounter > 0) {
		bg = buttonActiveBGColor;
		--pressCounter;
	} else if(doPress) {
		doUpdate();
	} else {
		isInvalidated = false;
	}
	if(_backgroundSet) {
		ulxDrawFillRect(x+_x, y+_y, x+_x+_w, y+_y+_h, bg);
	}
	if(_borderSet) {
		ulxDrawRect(x+_x, y+_y, x+_x+_w, y+_y+_h, _border);
	}
	s16 sWidth = (_w>>1) - (ulxGetStringLength(_str)>>1);
	s16 sHeight = (_h>>1) - (ulxGetFontHeight()>>1);

	if(_colorSet) ulxSetTextColor(_color);
	ulxDrawString(x+_x+sWidth,y+_y+sHeight,_str);

	//A button is a elementry lowest level
	//widget, meaning it cannot have children
	//which are draw and etc
}

void Button::onClick(s16 x, s16 y) {
	if(!checkBounds(x,y) || !isActive) return;
	_press();
	//doPress = true;
}
void Button::onControl(u32 keyPress) {
	if(!isActive) return;
	if(_keyPress != 0) {
		if(keyPress & _keyPress) {
			_press();
			//doPress = true;
		}
	}
}

void Button::doUpdate() {
	if(doPress){
		if(function != NULL) (*function)();
		if(functionoid != NULL) functionoid->execute();
		doPress = false;
	//	_press();
	}
	//Widget::doUpdate();
}
