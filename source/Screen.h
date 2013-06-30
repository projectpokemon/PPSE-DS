/*
 * Screen.h
 *
 *  Created on: Mar 7, 2010
 *      Author: Chase
 */

#ifndef SCREEN_H_
#define SCREEN_H_

class Screen {
protected:
	bool nextScreenCheck;
public:
	Image *top;
	Image *bottom;

	Screen();
	virtual ~Screen();

	void setTop(Image*w);
	void setBottom(Image*w);

	virtual void drawTop();
	virtual void drawBottom();

	virtual bool changeScreen();
	virtual Screen *nextScreen();

	//Should be called automatically after transition
	virtual void previous(Screen *n) { }
};

#endif /* SCREEN_H_ */
