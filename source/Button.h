/*
 * Button.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Chase
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Funct.h"

enum {
	buttonHighlightTime = 4,
	buttonBorderColor = RGB5(15,15,15),
	buttonInactiveBGColor = RGB5(31,31,31),
	buttonActiveBGColor	= RGB5(18,18,18),
};

class Button: public Label {
protected:
	void (*function)();
	FunctPtr functionoid;
	s8 pressCounter;
	u32 _keyPress;
	bool _canPress;
	u8 highlightTime;

	bool doPress;
	virtual void _press();
public:
	Button(int x, int y, int w, int h, const char *str);
	virtual ~Button();
	virtual void draw(int x, int y);
	virtual inline void setFunction(void (*funPtr)()) { function = funPtr; }
	virtual inline void setFunctionoid(FunctPtr foidPtr) { functionoid = foidPtr; }
	inline void bindControl(u32 keyComb) { _keyPress = keyComb; }
	virtual void onClick(s16 x, s16 y);
	virtual void onControl(u32 keyPress);
	inline void setClickable(bool v) { _canPress = v; }
	inline void setHighlightTime(u8 time) { highlightTime = time; }
	virtual void doUpdate();
	//no children
	inline void addChild(Widget*w) {}
};

#endif /* BUTTON_H_ */
