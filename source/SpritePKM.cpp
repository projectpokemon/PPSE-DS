/*
 * SpritePKM.cpp
 *
 *  Created on: 2010/02/04
 *      Author: Chase
 */

#include <cctype>
#include <stdio.h>
#include "Toolkit.h"

SpritePKM::SpritePKM(int x, int y, int num, bool shine) : Image(x, y, 80, 80, NULL) {
	setSpriteNumber(num, shine);
}

SpritePKM::~SpritePKM() {
	ulxDeleteImage(_texture->image);
	_texture->image = NULL;
	ulxDropTexture(_texture);
	free(_texture);
	_texture = NULL;
	_img = NULL;
}

void SpritePKM::setSpriteNumber(int num, bool shine) {
	if(_texture) {
		ulxDeleteImage(_texture->image);
		_texture->image = NULL;
		ulxDropTexture(_texture);
		free(_texture);
		_texture = NULL;
		_img = NULL;
	}

	char tmp[48];
	if(num == -1) {
		siprintf(tmp,"nitro:/sprite/special/EGG_%i.gif",shine?1:0);
	} else {
		siprintf(tmp,"nitro:/sprite/%03i_%i.gif",num,shine?1:0);
	}
	u8 *data = loadFile(tmp);
	_img = forceSpriteResize(ulxLoadImageGIF(data,ULX_PAL4));
	free(data);
	_texture = ulxCreateTexture(_img);
}

void SpritePKM::draw(int x, int y) {
	if(!isVisible) return;

	u8 palTMP = ulxFirstPaletteTransparent;
	ulxFirstPaletteTransparent = 1;
	Image::draw(x,y);
	ulxFirstPaletteTransparent = palTMP;

	//Widget::draw(x+_x,y+_y);
}
