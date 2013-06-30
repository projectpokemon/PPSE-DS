/*
 * Widget.cpp
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include "Widget.h"

Widget::Widget(int x, int y, int w, int h) {
	parent = NULL;
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	isInvalidated = true;
	isActive = true;
	isVisible = true;
}

Widget::~Widget() {
	for(u16 i=0; i<children.size(); ++i) {
		delete children[i];
	}
	children.clear();
}

const s16 Widget::getX() const {
	s16 x = 0;
	if(parent != NULL) x = parent->getX();
	return x+_x;
}

const s16 Widget::getY() const {
	s16 y = 0;
	if(parent != NULL) y = parent->getY();
	return y+_y;
}

void Widget::onClick(s16 x, s16 y) {
	if(!checkBounds(x,y) || !isActive) return;
	_cx = x; _cy = y;
	bool active[children.size()];
	for(u16 i=0; i<children.size(); ++i) {
		active[i] = children[i]->isActive;
	}
	for(u16 i=0; i<children.size(); ++i) {
		if(active[i]) children[i]->onClick(x, y);
	}
}

void Widget::onDrag(s16 x, s16 y) {
	if(!checkBounds(_cx,_cy) || !isActive) return;
	bool active[children.size()];
	for(u16 i=0; i<children.size(); ++i) {
		active[i] = children[i]->isActive;
	}
	for(u16 i=0; i<children.size(); ++i) {
		if(active[i]) children[i]->onDrag(x, y);
	}
}

void Widget::onDragEnd() {
	if(!checkBounds(_cx,_cy) || !isActive) return;
	bool active[children.size()];
	for(u16 i=0; i<children.size(); ++i) {
		active[i] = children[i]->isActive;
	}
	for(u16 i=0; i<children.size(); ++i) {
		if(active[i]) children[i]->onDragEnd();
	}
}


bool Widget::checkBounds(int x, int y) {
	int tx = getX();
	int ty = getY();
	if(x > tx && y > ty && x < tx+_w && y < ty+_h) {
		return true;
	}
	return false;
}

bool Widget::checkInvalidated() {
	if(isInvalidated) return true;
	for(u16 i=0; i<children.size(); ++i) {
		if(children[i]->checkInvalidated())
			return true;
	}
	return false;
}

void Widget::onControl(u32 keyPress) {
	if(!isActive) return;
	bool active[children.size()];
	for(u16 i=0; i<children.size(); ++i) {
		active[i] = children[i]->isActive;
	}
	for(u16 i=0; i<children.size(); ++i) {
		if(active[i]) children[i]->onControl(keyPress);
	}
}

//void Widget::doUpdate() {
//	if(!isActive) return;
//	bool active[children.size()];
//	for(u16 i=0; i<children.size(); ++i) {
//		active[i] = children[i]->isActive;
//	}
//	for(u16 i=0; i<children.size(); ++i) {
//		if(active[i]) children[i]->doUpdate();
//	}
//}

void Widget::draw(int x, int y) {
	isInvalidated = false;
	if(!isVisible) return;
	bool active[children.size()];
	for(u16 i=0; i<children.size(); ++i) {
		active[i] = children[i]->isVisible;
	}
	for(u16 i=0; i<children.size(); ++i) {
		if(active[i]) children[i]->draw(_x+x,_y+y);
	}
}
