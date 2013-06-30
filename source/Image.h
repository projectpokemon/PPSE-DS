/*
 * Image.h
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

class Image : public Widget {
public:
	ulxImage * _img;
	ulxTexture *_texture;
	Image(int x, int y, int w, int h, ulxImage *img);
	virtual ~Image();
	void createTexture();
	void dropTexture();
	void setTexture(ulxImage*img);
	virtual void draw(int x, int y);
};

#endif /* __IMAGE_H_ */
