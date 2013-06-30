/*
 * Image.cpp
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */


#include <nds.h>
#include <vector>
#include "Widget.h"
#include <ulx.h>
#include "Image.h"

Image::Image(int x, int y, int w, int h, ulxImage *img) : Widget(x,y,w,h) {
	_img = img;
	_texture = NULL;
}
Image::~Image() {
	dropTexture();
}
void Image::createTexture() {
	_texture = ulxCreateTexture(_img);
}
void Image::dropTexture() {
	if(_texture != NULL) {
		ulxDropTexture(_texture);
		free(_texture);
		_texture = NULL;
	}
}
void Image::setTexture(ulxImage*img) {
	isInvalidated = true;
	if(_texture != NULL) {
		ulxDropTexture(_texture);
		free(_texture);
		_texture = NULL;
	}
	_img = img;
//	_texture = ulxCreateTexture(_img);
}
void Image::draw(int x, int y) {
	if(!isVisible) return;
	if(_texture == NULL && _img != NULL) {
		createTexture();
	}
	//draw us
	ulxDrawTexture(x+_x,y+_y,_texture);
	//draw children
	Widget::draw(x+_x,y+_y);
}
