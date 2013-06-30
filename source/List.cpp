/*
 * List.cpp
 *
 *  Created on: Mar 7, 2010
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include "Widget.h"
#include <ulx.h>
#include "Funct.h"
#include "SliderV.h"
#include "List.h"

void List::_click(uint32 n) {
	if(function != NULL) (*function)(n);
	if(functionoid != NULL) functionoid->execute(n);
}

List::List(int x, int y, int w, int lines) : Widget(x,y,w,lines*12+2) {
	slide = new SliderV(w-28, 0, 28, lines*12+2);
	slide->setWindowLength(lines);
	slide->setDocumentLength(1);

	this->size = lines;
	this->addChild(slide);

	cursor = 0;
}

List::~List() {
	clear();
}

void List::onControl(u32 keyPress) {
	if(!isActive) return;
	int pos = slide->getWindowPosition();
	if(keyPress & KEY_A) {
		_click(slide->getWindowPosition()+cursor);
	} else if(keyPress & KEY_UP) {
		if(cursor == 0) {
			this->slide->setWindowPosition(pos-1);
		} else {
			--cursor;
		}
		this->isInvalidated = true;
	} else if(keyPress & KEY_DOWN) {
		if(cursor + pos + 1 < list.size()) {
			if(cursor == size-1) {
				this->slide->setWindowPosition(pos+1);
			} else {
				++cursor;
			}
			this->isInvalidated = true;
		}
	}
	if(keyPress & KEY_LEFT) {
		//to top
		cursor = 0;
		this->isInvalidated = true;
	} else if(keyPress & KEY_RIGHT) {
		//to bottom
		if(list.size() < size) {
			cursor = list.size()-1;
		} else {
			cursor = size-1;
		}
		this->isInvalidated = true;
	}
	Widget::onControl(keyPress);
}

void List::onClick(s16 x, s16 y) {
	if(!checkBounds(x,y) || !isActive) return;
	//Make sure it is only in our list window...
	if(x < _x+_w-32) {
		uint32 tmp = (y-_y)/12;
		if(tmp == cursor) {
			_click(slide->getWindowPosition()+cursor);
		}
		cursor = tmp;
		this->isInvalidated = true;
	}
	Widget::onClick(x,y);
}

void List::addEntry(const char *str, uint16 color) {
	Entry *ent = new Entry(str,color);
	list.push_back(ent);
	if(list.size() > size) {
		slide->setDocumentLength(list.size() - size);
	}
}

void List::clear() {
	slide->setDocumentLength(1);
	list.clear();
}

void List::draw(int x, int y) {
	if(!isVisible) return;

	//draw our window
	ulxDrawFillRect(x+_x, y+_y, x+_x+_w-32, y+_y+_h, listBGColor);
	ulxDrawRect(x+_x, y+_y, x+_x+_w-32, y+_y+_h, listBorderColor);

	//Draw the current list
	uint32 pos = slide->getWindowPosition();
	for(uint32 i=0; i<list.size() && i < size; ++i) {
		if(i == cursor) {
			ulxDrawFillRect(x+_x+1, y+_y+1+i*12, x+_x+_w-33, y+_y+13+i*12, listHighlightColor);
		}
		ulxSetTextColor(list[i+pos]->color);
		ulxDrawString(x+_x+2,y+_y+2+i*12, list[i+pos]->string);
	}
	Widget::draw(x,y);
}
