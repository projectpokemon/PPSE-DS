/*
 * Slider.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include "Widget.h"
#include <ulx.h>
#include "SliderV.h"

#include <nds/arm9/videoGL.h>
#include <nds/arm9/math.h>


void SliderV::_ret() {
	if(scrollFunPtr != NULL) scrollFunPtr(windowPosition);
	if(sFunctionoid != NULL) sFunctionoid->execute(windowPosition);
}

SliderV::SliderV(int x, int y, int w, int h) : Widget(x,y,w,h) {
	_trackLength=0;
	_gripLength=0;
	_gripPosition=0;
	_wLength=0;
	_dLength=0;
	windowPosition = 0;
	sFunctionoid = NULL;
	scrollFunPtr = NULL;
}

SliderV::~SliderV() {
}

void SliderV::_calcGripLength() {
	_trackLength = _h-2;

	int sliderMaxGripLength = _trackLength;

	//fixed point 20.12 division and multiplication
	//videoGL.h and math.h6
	int32 gripRatio = divf32(inttof32(_wLength),inttof32(_dLength));
	_gripLength = f32toint(mulf32(inttof32(_trackLength),gripRatio));
	//Done with fixed point

	if(_gripLength < sliderMinGripLength) {
		//_trackLength -= (sliderMinGripLength - _gripLength);
		_gripLength = sliderMinGripLength;
	}
	if(_gripLength > sliderMaxGripLength) _gripLength = sliderMaxGripLength;
}
void SliderV::_calcGripPosition() {
	_scrollSize = _trackLength - _gripLength;

	//fixed point 20.12 division and multiplication via videoGL.h and math.h
	//-----
	//Divide the current position by the length of the document we are scrolling
	int32 scrollRatio = divf32(inttof32(windowPosition),inttof32(_dLength));
	//the grip position is obtained by multiplying the ratio of location on the document
	//to the document length by the area scrollable (track length - grip lenght)
	_gripPosition = f32toint(mulf32(inttof32(_scrollSize),scrollRatio));
	//-----
	//Done with fixed point
}
void SliderV::setWindowLength(int n) {
	_wLength = n;
	if(_dLength != 0) {
		_calcGripLength();
		_calcGripPosition();
	}
}
void SliderV::setDocumentLength(int n) {
	_dLength = n;
	if(_wLength != 0) {
		_calcGripLength();
		_calcGripPosition();
	}
}

void SliderV::setWindowPosition(int n) {
	windowPosition = n;
	if(windowPosition < 0) windowPosition = 0;
	if(_wLength != 0 && _dLength != 0) {
		if(windowPosition > _dLength) windowPosition = _dLength;

		_ret();

		_calcGripPosition();
		isInvalidated = true;
	}
}

bool SliderV::checkGripBounds(int x, int y) {
	int tx = getX();
	int ty = getY();
	if(x > tx && y > ty+_gripPosition
	&& x < tx+_w && y < ty+_gripPosition+_gripLength) {
		return true;
	}
	return false;
}

void SliderV::onClick(s16 x, s16 y) {
	if(!checkBounds(x,y) || !isActive) return;
	if(checkGripBounds(x,y)) {
		_cx = x; _cy = y;
		return;
	}
	int nWinPos = windowPosition;
	int ny = getY() +  _gripPosition + (_gripLength>>1);
	if(y < ny) {
		nWinPos -= _wLength;
		if(nWinPos < 0) nWinPos = 0;
	} else {
		nWinPos += _wLength;
		if(nWinPos > _dLength) nWinPos = _dLength;
	}
	if(nWinPos != windowPosition) {
		windowPosition = nWinPos;

		_ret();

		_calcGripPosition();
		isInvalidated = true;
	}
}

void SliderV::onDrag(s16 x, s16 y) {
	if( !checkBounds(_cx,_cy) || !isActive ) return;
	if( checkGripBounds(_cx,_cy) && checkBounds(x,y) ) {
		s16 delta = y - _cy;
		if(delta != 0) {
			int nGripPos = _gripPosition + delta;
			//Limit it so it is not out of bounds
			if(nGripPos < 0) nGripPos = 0;
			if(nGripPos > _scrollSize) nGripPos = _scrollSize;

			//Just reverse the algorithms we did before to get back
			//fixed point 20.12 division and multiplication
			//videoGL.h and math.h
			int32 nScrollRatio = divf32(inttof32(nGripPos),inttof32(_scrollSize));
			windowPosition = f32toint(mulf32(inttof32(_dLength),nScrollRatio));
			//Done with fixed point

			if(windowPosition < 0) windowPosition = 0;

			_ret();

			_calcGripPosition();
			isInvalidated = true;
		}
	}
	_cx = x;
	_cy = y;
}

void SliderV::draw(int x, int y) {
	if(!isVisible) return;

	ulxDrawFillRect(
			x+_x,
			y+_y,
			x+_x+_w,
			y+_y+_h,
			sliderBGColor);
	ulxDrawRect(
			x+_x,
			y+_y,
			x+_x+_w,
			y+_y+_h, sliderBorderColor);

	ulxDrawFillRect(x+_x+2,
			y+_y+2+_gripPosition,
			x+_x+_w-2,
			y+_y+_gripPosition+_gripLength,
			sliderInactiveGripColor);
	isInvalidated = false;
//	s16 sWidth = (_w>>1) - (ulxGetStringLength(_str)>>1);
//	s16 sHeight = (_h>>1) - (ulxGetFontHeight()>>1);
//
//	if(_colorSet) ulxSetTextColor(_color);
//	ulxDrawString(x+_x+sWidth,y+_y+sHeight,_str);
}

void SliderV::onControl(u32 keyPress) {
	if(!isActive) return;
	//left and right are equivalent to the click jumps
	int nWinPos = windowPosition;
	if(keyPress & KEY_LEFT) {
		nWinPos -= _wLength;
		if(nWinPos < 0) nWinPos = 0;
	}
	if(keyPress & KEY_RIGHT) {
		nWinPos += _wLength;
		if(nWinPos > _dLength) nWinPos = _dLength;
		if(_dLength < _wLength) nWinPos = 0;
	}
	if(nWinPos != windowPosition) {
		windowPosition = nWinPos;

		_ret();

		_calcGripPosition();
		isInvalidated = true;
	}
}
