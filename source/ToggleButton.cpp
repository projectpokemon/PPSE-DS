/*
 * ToggleButton.cpp
 *
 *  Created on: 2010/02/06
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include <ulx.h>
#include "Widget.h"
#include "Label.h"
#include "Button.h"
#include "ToggleButton.h"

void ToggleButton::_press() {
	if(!_canPress) return;
	_pressed = !_pressed;
	if(function != NULL) (*function)();
	if(functionoid != NULL) functionoid->execute();
	if(numfunct != NULL) (*numfunct)(_pressed ? 1 : 0);
	if(numFunctionoid != NULL) numFunctionoid->execute(_pressed ? 1 : 0);
	isInvalidated = true;
}

ToggleButton::ToggleButton(int x, int y, int w, int h, const char *str) : Button(x,y,w,h,str) {
	_pressed = false;
	numfunct = NULL;
	numFunctionoid = NULL;
}

ToggleButton::~ToggleButton() {

}

void ToggleButton::draw(int x, int y) {
	if(!isVisible) return;
	rgb bg = buttonInactiveBGColor;
	if(_pressed) bg = buttonActiveBGColor;
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
}
