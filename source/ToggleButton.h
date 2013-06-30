/*
 * ToggleButton.h
 *
 *  Created on: 2010/02/06
 *      Author: Chase
 */

#ifndef TOGGLEBUTTON_H_
#define TOGGLEBUTTON_H_

#include "Button.h"

class ToggleButton: public Button {
	FunctNumPtr numFunctionoid;
	void (*numfunct)(int32);
	bool _pressed;
	virtual void _press();
public:
	ToggleButton(int x, int y, int w, int h, const char *str);
	virtual ~ToggleButton();
	inline bool isPressed() { return _pressed; }
	inline void setPressed(bool press) { _pressed = press; }
	virtual inline void setNumFunction(void (*funPtr)(int32)) { numfunct = funPtr; }
	virtual inline void setNumFunctionoid(FunctNumPtr fun) { numFunctionoid = fun; }
	virtual void draw(int x, int y);
};

#endif /* TOGGLEBUTTON_H_ */
