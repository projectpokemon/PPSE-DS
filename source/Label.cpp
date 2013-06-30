/*
 * Label.cpp
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include "Widget.h"
#include <ulx.h>
#include "Label.h"
#include <cstring>

Label::Label(int x, int y, int w, int h, const char *str)  : Widget(x,y,w,h) {
	_align = 0;
	_str = NULL;
	_colorSet = false;
	_color = 0;
	_wordWrap = false;
	_backgroundSet = false;
	_borderSet = false;
	_border = labelBorderColor;
	_background = labelBackgroundColor;
	setText(str);
}

Label::Label(int x, int y, int w, int h, const char *str, bool wrap)  : Widget(x,y,w,h) {
	_align = 0;
	_str = NULL;
	_colorSet = false;
	_color = 0;
	_wordWrap = wrap;
	_backgroundSet = false;
	_borderSet = false;
	_border = labelBorderColor;
	_background = labelBackgroundColor;
	setText(str);
}

Label::~Label() {
	free(_str);
}

void Label::setText(const char *str) {
	if(str != NULL && strcmp(str,_str) != 0) {
		if(_str != NULL) {
			free(_str);
			_str = NULL;
		}
		_str = (char*)malloc(strlen(str)+1);
		memset(_str,0,strlen(str)+1);
		strcpy(_str,str);
		_slen = ulxGetStringLength(_str);
		isInvalidated = true;
	}
}

void Label::setColor(rgb color) {
	_color = color;
	_colorSet = true;
}

void Label::setBackgroundColor(rgb color) {
	_background = color;
}
void Label::setBorderColor(rgb color) {
	_border = color;
}

void Label::draw(int x, int y) {
	if(!isVisible) return;
	//draw us
	if(_colorSet) ulxSetTextColor(_color);
	if(_backgroundSet) {
		ulxDrawFillRect(x+_x, y+_y, x+_x+_w, y+_y+_h, _background);
	}
	if(_borderSet) {
		ulxDrawRect(x+_x, y+_y, x+_x+_w, y+_y+_h, _border);
	}

	if(_wordWrap) {

		uint16 flen = strlen(_str);
		uint16 start = 0;
		uint16 line = 0;
		while(true) {
			uint16 nlen = flen - start;
			//determine max length for this row
			bool broken = false;
			uint16 best = 0;
			for(int j = 0; j < nlen; ++j) {
				uint16 len = ulxGetStringLengthN(&_str[start],j);
				if(_str[start+j] == ' ') best = j+1;
				if(len > _w) {
					broken = true;
					break;
				}
			}

			if(!broken) {
				uint16 len = flen-start+1;
				char tmp[len];
				memset(tmp,0,len);
				strncpy(tmp,&_str[start],len-1);
				uint16 llen = ulxGetStringLengthN(&_str[start],len);
				//the rest of the line
				switch(_align) {
					case ALIGN_LEFT: {
						ulxDrawString(x+_x,y+_y+line*12, tmp);
						break;
					}
					case ALIGN_CENTER: {
						ulxDrawString(x+_x + (_w>>1)-(llen>>1),y+_y+line*12, tmp);
						break;
					}
					case ALIGN_RIGHT: {
						ulxDrawString(x+_x + _w-llen,y+_y+line*12, tmp);
						break;
					}
				}
				break;
			} else {
				uint16 len = best+1;
				char tmp[len];
				memset(tmp,0,len);
				strncpy(tmp,&_str[start],len-1);
				uint16 llen = ulxGetStringLengthN(&_str[start],len);
				start += best;
				switch(_align) {
					case ALIGN_LEFT: {
						ulxDrawString(x+_x,y+_y+line*12, tmp);
						break;
					}
					case ALIGN_CENTER: {
						ulxDrawString(x+_x + (_w>>1)-(llen>>1),y+_y+line*12, tmp);
						break;
					}
					case ALIGN_RIGHT: {
						ulxDrawString(x+_x + _w-llen,y+_y+line*12, tmp);
						break;
					}
				}
				++line;
				//if(line == 2) break;
			}
		}

		Widget::draw(x+_x,y+_y);
		return;
	}

	switch(_align) {
		case ALIGN_LEFT: {
			ulxDrawString(x+_x,y+_y, _str);
			break;
		}
		case ALIGN_CENTER: {
			ulxDrawString(x+_x + (_w>>1)-(_slen>>1),y+_y, _str);
			break;
		}
		case ALIGN_RIGHT: {
			ulxDrawString(x+_x + _w-_slen,y+_y, _str);
			break;
		}
	}
	//draw children
	Widget::draw(x+_x,y+_y);
}

void Label::setAlignment(Alignment a) {
	_align = (int)a;
}
