/*
 * SpritePKM.h
 *
 *  Created on: 2010/02/04
 *      Author: Chase
 */

#ifndef SPRITEPKM_H_
#define SPRITEPKM_H_

class SpritePKM: public Image {
public:
	SpritePKM(int x, int y, int num, bool shine);
	virtual ~SpritePKM();
	void setSpriteNumber(int num, bool shine);
	virtual void draw(int x, int y);
};

#endif /* SPRITEPKM_H_ */
