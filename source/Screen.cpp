/*
 * Screen.cpp
 *
 *  Created on: Mar 7, 2010
 *      Author: Chase
 */

#include <nds.h>
#include <vector>
#include <ulx.h>
#include "Widget.h"
#include "Image.h"
#include "Screen.h"

Screen::Screen() {
	nextScreenCheck = false;
}

Screen::~Screen() {
	top->_texture = NULL;
	bottom->_texture = NULL;
	delete top;
	delete bottom;
}

void Screen::setTop(Image*w) {
	top = new Image(w->getX(),w->getY(),w->getWidth(),w->getHeight(),NULL);
	top->_texture = w->_texture;
}
void Screen::setBottom(Image*w) {
	bottom = new Image(w->getX(),w->getY(),w->getWidth(),w->getHeight(),NULL);
	bottom->_texture = w->_texture;
}

void Screen::drawTop() {
	top->draw(0,0);
}
void Screen::drawBottom() {
	bottom->draw(0,0);
}
bool Screen::changeScreen() {
	return nextScreenCheck;
}
Screen *Screen::nextScreen() {
	return this;
}
